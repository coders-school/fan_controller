#include "ControllerWithDisplay.hpp"
#include "FanMock.hpp"
#include "LcdDisplayMock.hpp"
#include "ThermometerMock.hpp"

#include "gtest/gtest.h"

namespace {
using testing::AllOf;
using testing::HasSubstr;
using testing::NiceMock;
using testing::Return;
using testing::TestWithParam;
using testing::ValuesIn;

constexpr double TARGET_TEMPERATURE = 36.6;
constexpr double TOLERANCE = 0.5;
constexpr int FAN_NOMINAL_SPEED = 1000;
constexpr int FAN_MAX_SPEED = 3000;

static constexpr double MAX_TEMP_BEFORE_SPEED_INCREASE = TARGET_TEMPERATURE + TOLERANCE;
static constexpr double TEMP_DIFF_FOR_ONE_RPM = 0.001;

class ControllerWithDisplayWithUsedMocks
{
  public:
    ControllerWithDisplayWithUsedMocks()
        : thermometer_(std::make_unique<NiceMock<ThermometerMock>>())
        , fan_(std::make_unique<NiceMock<FanMock>>())
        , display_(std::make_unique<NiceMock<LcdDisplayMock>>())
        , sut_(*thermometer_, *fan_, TARGET_TEMPERATURE, TOLERANCE, *display_)
    { }

    std::unique_ptr<ThermometerMock> thermometer_;
    std::unique_ptr<FanMock> fan_;
    std::unique_ptr<LcdDisplayMock> display_;
    ControllerWithDisplay sut_;
};

class ControllerWithDisplayShould : public TestWithParam<double>,
                                    public ControllerWithDisplayWithUsedMocks
{
  public:
    ControllerWithDisplayShould()
        : ControllerWithDisplayWithUsedMocks()
    { }
};

double temperatures[] = {
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

int calculateRPMaboveNominal(double temperature)
{
    double tempDifference = (temperature - MAX_TEMP_BEFORE_SPEED_INCREASE);
    int rpmIncrease { 0 };
    if (tempDifference > 0.0) {
        rpmIncrease = static_cast<int>(std::round(tempDifference / TEMP_DIFF_FOR_ONE_RPM));
    }
    int final_speed = FAN_NOMINAL_SPEED + rpmIncrease;

    return final_speed > FAN_MAX_SPEED ? FAN_MAX_SPEED
                                       : final_speed;
}

int calcDesiredRpm(double temperature)
{
    if (temperature < TARGET_TEMPERATURE - TOLERANCE) {
        return 0;
    }
    else if (temperature >= TARGET_TEMPERATURE - TOLERANCE
             && temperature <= TARGET_TEMPERATURE + TOLERANCE) {
        return FAN_NOMINAL_SPEED;
    }

    return calculateRPMaboveNominal(temperature);
}

INSTANTIATE_TEST_SUITE_P(additionalyToBaseControllerUtilities,
                         ControllerWithDisplayShould,
                         ValuesIn(temperatures));

TEST_P(ControllerWithDisplayShould, displayTemperatureAndCurrentRpm)
{
    const double tempRead = GetParam();
    const std::string tempStr = std::to_string(tempRead);

    const int newRPM = calcDesiredRpm(tempRead);
    const std::string newRPMstr = std::to_string(newRPM);

    EXPECT_CALL(*thermometer_, getTemperature()).WillRepeatedly(Return(tempRead));
    EXPECT_CALL(*fan_, getSpeed()).WillOnce(Return(FAN_NOMINAL_SPEED)).WillOnce(Return(newRPM));
    EXPECT_CALL(*display_,
                print(AllOf(HasSubstr("Temperature"),
                            HasSubstr(tempStr),
                            HasSubstr("RPM"),
                            HasSubstr(newRPMstr))));

    sut_.updateRpm();
    sut_.displayInfo();
}

}   // namespace
