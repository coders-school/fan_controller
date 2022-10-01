#include "Controller.hpp"
#include "FanMock.hpp"
#include "ThermometerMock.hpp"

#include "gtest/gtest.h"

namespace controller_ut {

using testing::_;
using testing::NiceMock;
using testing::Return;
using testing::Test;
using testing::TestWithParam;
using testing::ValuesIn;

constexpr double TARGET_TEMPERATURE = 36.6;
constexpr double TOLERANCE = 0.5;
constexpr int FAN_NOMINAL_SPEED = 1000;
constexpr int FAN_MAX_SPEED = 3000;

static constexpr double MAX_TEMP_BEFORE_SPEED_INCREASE = TARGET_TEMPERATURE + TOLERANCE;
static constexpr double TEMP_DIFF_FOR_ONE_RPM = 0.001;

int calcDesiredRpmSpeedForTemp(double temperature)
{
    double tempDifference = (temperature - MAX_TEMP_BEFORE_SPEED_INCREASE);
    int speedIncrease { 0 };
    if (tempDifference > 0.0) {
        speedIncrease = static_cast<int>(std::round(tempDifference / TEMP_DIFF_FOR_ONE_RPM));
    }

    int final_speed = FAN_NOMINAL_SPEED + speedIncrease;

    return final_speed > 3000 ? 3000
                              : final_speed;
}

class SutWithDependantMocks
{
  protected:
    SutWithDependantMocks()
        : thermometer_(std::make_unique<NiceMock<ThermometerMock>>())
        , fan_(std::make_unique<NiceMock<FanMock>>())
        , sut_(*thermometer_, *fan_, TARGET_TEMPERATURE, TOLERANCE)
    { }

    std::unique_ptr<ThermometerMock> thermometer_;
    std::unique_ptr<FanMock> fan_;
    Controller sut_;
};

class ForTemperatureValuesWithinToleranceRange : public TestWithParam<double>,
                                                 public SutWithDependantMocks
{
  public:
    ForTemperatureValuesWithinToleranceRange()
        : SutWithDependantMocks()
    { }
};

class ForTemperatureValuesBelowToleranceRange : public TestWithParam<double>,
                                                public SutWithDependantMocks
{
  public:
    ForTemperatureValuesBelowToleranceRange()
        : SutWithDependantMocks()
    { }
};

class ForTemperatureValuesAboveToleranceRange : public TestWithParam<double>,
                                                public SutWithDependantMocks
{
  public:
    ForTemperatureValuesAboveToleranceRange()
        : SutWithDependantMocks()
    { }
};

class ForTemperatureValuesAboveMaximumFanSpeed : public TestWithParam<double>,
                                                 public SutWithDependantMocks
{
  public:
    ForTemperatureValuesAboveMaximumFanSpeed()
        : SutWithDependantMocks()
    { }
};

double tempsWihtinThreshold[] = {
    TARGET_TEMPERATURE - TOLERANCE,
    TARGET_TEMPERATURE - TOLERANCE + 0.001,
    TARGET_TEMPERATURE + TOLERANCE - 0.001,
    TARGET_TEMPERATURE - TOLERANCE * 0.99,
    TARGET_TEMPERATURE - TOLERANCE * 0.5,
    TARGET_TEMPERATURE - TOLERANCE * 0.3,
    TARGET_TEMPERATURE + TOLERANCE * 0.3,
    TARGET_TEMPERATURE + TOLERANCE * 0.5,
    TARGET_TEMPERATURE + TOLERANCE * 0.99,
    TARGET_TEMPERATURE + TOLERANCE
};

double tempsBelowTemperatureThreshold[] {
    -40.0,
    -30.0,
    -20.0,
    -10.0,
    -0.001,
    0.0,
    0.001,
    2.0,
    4.0,
    TARGET_TEMPERATURE - TOLERANCE - 0.5,
    TARGET_TEMPERATURE - TOLERANCE - 0.1,
    TARGET_TEMPERATURE - TOLERANCE - 0.001,
    TARGET_TEMPERATURE - TOLERANCE - 0.0005,
};

double tempsAboveTemperatureThreshold[] {
    TARGET_TEMPERATURE + TOLERANCE + 0.0005,
    TARGET_TEMPERATURE + TOLERANCE + 0.001,
    TARGET_TEMPERATURE + TOLERANCE + 0.002,
    TARGET_TEMPERATURE + TOLERANCE + 0.003,

    TARGET_TEMPERATURE + TOLERANCE + 0.005,
    TARGET_TEMPERATURE + TOLERANCE + 0.012,
    TARGET_TEMPERATURE + TOLERANCE + 0.088,
    TARGET_TEMPERATURE + TOLERANCE + 0.1,
    TARGET_TEMPERATURE + TOLERANCE + 0.105,
    TARGET_TEMPERATURE + TOLERANCE + 0.150,
    TARGET_TEMPERATURE + TOLERANCE + 0.320,
    TARGET_TEMPERATURE + TOLERANCE + 0.700,
    TARGET_TEMPERATURE + TOLERANCE + 0.999,
    TARGET_TEMPERATURE + TOLERANCE + 1.1,
    TARGET_TEMPERATURE + TOLERANCE + 1.5,
    TARGET_TEMPERATURE + TOLERANCE + 1.75,
    TARGET_TEMPERATURE + TOLERANCE + 1.995,
    TARGET_TEMPERATURE + TOLERANCE + 2.01,
    TARGET_TEMPERATURE + TOLERANCE + 2.520,
};

double tempsAboveFanMaxSpeed[] {
    TARGET_TEMPERATURE + TOLERANCE + 3.25,
    TARGET_TEMPERATURE + TOLERANCE + 3.95,
    TARGET_TEMPERATURE + TOLERANCE + 4.3,
    TARGET_TEMPERATURE + TOLERANCE + 5.5,
    TARGET_TEMPERATURE + TOLERANCE + 9.5,
    TARGET_TEMPERATURE + TOLERANCE + 20.5,
    TARGET_TEMPERATURE + TOLERANCE + 21.5,
};

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesWithinToleranceRange,
                         ValuesIn(tempsWihtinThreshold));

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesBelowToleranceRange,
                         ValuesIn(tempsBelowTemperatureThreshold));

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesAboveToleranceRange,
                         ValuesIn(tempsAboveTemperatureThreshold));

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesAboveMaximumFanSpeed,
                         ValuesIn(tempsAboveFanMaxSpeed));

TEST_P(ForTemperatureValuesBelowToleranceRange, disableFanIfWasRunning)
{
    const double tempRead = GetParam();
    // disabling from above nominal speed
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1500));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();

    // disabling from  nominal speed
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1000));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesBelowToleranceRange, sendNoSteeringSignalIfFanWasDisabled)
{
    const double tempRead = GetParam();

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(0));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);
    EXPECT_CALL(*fan_, setSpeed(_)).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesWithinToleranceRange, shouldEnableFanIfWasNotRunning)
{
    const double tempRead = GetParam();

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(0));
    EXPECT_CALL(*fan_, enable());

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesWithinToleranceRange, shouldSendNoSteeringSignalIfFanWasAlreadyAtNominalSpeed)
{
    const double tempRead = GetParam();

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1000));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);
    EXPECT_CALL(*fan_, setSpeed(_)).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesWithinToleranceRange, shouldSetFanSpeedToNominalFromAboveNominal)
{
    const double tempRead = GetParam();

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1500));
    EXPECT_CALL(*fan_, setSpeed(FAN_NOMINAL_SPEED));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesAboveToleranceRange, shouldSetFanSpeedToDesiredIfItWasDisabled)
{

    const double tempRead = GetParam();
    int desiredRPM = calcDesiredRpmSpeedForTemp(tempRead);

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(0));
    EXPECT_CALL(*fan_, setSpeed(desiredRPM));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesAboveToleranceRange, shouldSetFanSpeedToDesiredIfItWasRunningAtNominalSpeed)
{
    const double tempRead = GetParam();
    int desiredRPM = calcDesiredRpmSpeedForTemp(tempRead);

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1000));
    EXPECT_CALL(*fan_, setSpeed(desiredRPM));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesAboveToleranceRange, shouldSetFanSpeedToDesiredIfItWasRunningAboveNominalSpeed)
{
    const double tempRead = GetParam();
    int desiredRPM = calcDesiredRpmSpeedForTemp(tempRead);

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1200));
    EXPECT_CALL(*fan_, setSpeed(desiredRPM));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesAboveMaximumFanSpeed, shouldSetFanSpeedToMaximum)
{
    const double tempRead = GetParam();

    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(1200));
    EXPECT_CALL(*fan_, setSpeed(FAN_MAX_SPEED));
    EXPECT_CALL(*fan_, disable()).Times(0);
    EXPECT_CALL(*fan_, enable()).Times(0);

    sut_.updateRpm();
}

}   // namespace controller_ut
