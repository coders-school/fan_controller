// TODO: VERIFY
//  #include "external/catch.hpp"
//  #include "external/hippomocks.h"
//  #include "Controller.hpp"
#include "Controller.hpp"
#include "FanMock.hpp"
#include "ThermometerMock.hpp"

#include "gtest/gtest.h"

namespace controller_ut {

using testing::NiceMock;
using testing::Return;
using testing::Test;
using testing::TestWithParam;
using testing::ValuesIn;

constexpr double TARGET_TEMPERATURE = 36.6;
constexpr double TOLERANCE = 0.5;

class ControllersUpdateRpmShould : public Test
{
  public:
    ControllersUpdateRpmShould();

  protected:
    std::unique_ptr<ThermometerMock> thermometer_;
    std::unique_ptr<FanMock> fan_;
    Controller sut_;
    const double fanStartTemperature_;
    const double nominalFanSpeedUpperThreshold_;
};

ControllersUpdateRpmShould::ControllersUpdateRpmShould()
    : thermometer_(std::make_unique<NiceMock<ThermometerMock>>())
    , fan_(std::make_unique<NiceMock<FanMock>>())
    , sut_(*thermometer_, *fan_, TARGET_TEMPERATURE, TOLERANCE)
    , fanStartTemperature_(TARGET_TEMPERATURE - TOLERANCE)
    , nominalFanSpeedUpperThreshold_(TARGET_TEMPERATURE + TOLERANCE)
{ }

class ControllerShould : public TestWithParam<double>
{
  public:
    ControllerShould()
        : thermometer_(std::make_unique<NiceMock<ThermometerMock>>())
        , fan_(std::make_unique<NiceMock<FanMock>>())
        , sut_(*thermometer_, *fan_, TARGET_TEMPERATURE, TOLERANCE)
    { }

  protected:
    std::unique_ptr<ThermometerMock> thermometer_;
    std::unique_ptr<FanMock> fan_;
    Controller sut_;
};

double tempsWihtinThreshold[] = {
    TARGET_TEMPERATURE - TOLERANCE + 0.001,
    TARGET_TEMPERATURE + TOLERANCE - 0.001,
    TARGET_TEMPERATURE - TOLERANCE * 0.99,
    TARGET_TEMPERATURE - TOLERANCE * 0.5,
    TARGET_TEMPERATURE - TOLERANCE * 0.3,
    TARGET_TEMPERATURE + TOLERANCE * 0.3,
    TARGET_TEMPERATURE + TOLERANCE * 0.5,
    TARGET_TEMPERATURE + TOLERANCE * 0.99,
};

INSTANTIATE_TEST_SUITE_P(ForTemperaturesValuesWithinToleranceRange,
                         ControllerShould,
                         ValuesIn(tempsWihtinThreshold));

// TODO: REMOVE
/*  TESTS to do

    DONE disable the fan when temperature is below target temparature - tolerance

- set start fan speed to 1000 when the temperature is within tolerance
- increase the fan speed 1 rpm for 0.001 degree above temperature + tolerance
- do not increase fan speed even when max allowed fan speed reached;

- test for not changing fan setting if conditions wont change enough


*/

TEST_F(ControllersUpdateRpmShould, disableFanForTemperatureBelowZero)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(-2.2));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

TEST_F(ControllersUpdateRpmShould, disableFanForTemperatureEqualToZero)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(0));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

TEST_F(ControllersUpdateRpmShould, disableFanForTemperatureSlightlyBellowStartThreshold)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(fanStartTemperature_ - 0.001));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

TEST_F(ControllersUpdateRpmShould, enableFanForTemperatureEqualToStartThreshold)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(fanStartTemperature_));
    EXPECT_CALL(*fan_, enable());

    sut_.updateRpm();
}

TEST_F(ControllersUpdateRpmShould, enableFanForTemperatureEqualToTargetTempPlusTolerance)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(nominalFanSpeedUpperThreshold_));
    EXPECT_CALL(*fan_, enable());

    sut_.updateRpm();
}

TEST_P(ControllerShould, enableFan)
{
    const double tempRead = GetParam();
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, enable());

    sut_.updateRpm();
}

}   // namespace controller_ut
