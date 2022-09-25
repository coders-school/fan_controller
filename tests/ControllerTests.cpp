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

constexpr double TARGET_TEMPERATURE = 36.6;
constexpr double TOLERANCE = 0.5;

class ControllerShould : public Test
{
  public:
    ControllerShould();
    // TODO: REMOVE
    // int getFanSpeedForTemp(const double temperatureValue) const;

  protected:
    std::unique_ptr<ThermometerMock> thermometer_;
    std::unique_ptr<FanMock> fan_;
    Controller sut_;
    const double fanStartTemperature_;
    const double increaseSpeedTemperature_;
};

ControllerShould::ControllerShould()
    : thermometer_(std::make_unique<NiceMock<ThermometerMock>>())
    , fan_(std::make_unique<NiceMock<FanMock>>())
    , sut_(*thermometer_, *fan_, TARGET_TEMPERATURE, TOLERANCE)
    , fanStartTemperature_(TARGET_TEMPERATURE - TOLERANCE)   // TODO: VERIFY if needed
    , increaseSpeedTemperature_(TARGET_TEMPERATURE + TOLERANCE)
{ }
// TODO: REMOVE
// int getFanSpeedForTemp(const double temperatureValue) const;
// TODO: REMOVE
/*  TESTS to do

- disable the fan when temperature is below target temparature - tolerance
- set start fan speed to 1000 when the temperature is within tolerance
- increase the fan speed 1 rpm for 0.001 degree above temperature + tolerance
- do not increase fan speed even when max allowed fan speed reached;


*/

TEST_F(ControllerShould, disableFanForTemperatureBelowZero)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(-2.2));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

TEST_F(ControllerShould, disableFanForTemperatureEqualToZero)
{
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(0));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

}   // namespace controller_ut