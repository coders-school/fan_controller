// TODO: VERIFY
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

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesWithinToleranceRange,
                         ValuesIn(tempsWihtinThreshold));

INSTANTIATE_TEST_SUITE_P(ControllerShould,
                         ForTemperatureValuesBelowToleranceRange,
                         ValuesIn(tempsBelowTemperatureThreshold));

// TODO: REMOVE
/*  TESTS to do

    DONE disable the fan when temperature is below target temparature - tolerance

- set start fan speed to 1000 when the temperature is within tolerance
- increase the fan speed 1 rpm for 0.001 degree above temperature + tolerance
- do not increase fan speed even when max allowed fan speed reached;

- test for not changing fan setting if conditions wont change enough


*/

TEST_P(ForTemperatureValuesWithinToleranceRange, shouldEnableFan)
{
    const double tempRead = GetParam();
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, enable());

    sut_.updateRpm();
}

TEST_P(ForTemperatureValuesBelowToleranceRange, disableFan)
{
    const double tempRead = GetParam();
    EXPECT_CALL(*thermometer_, getTemperature()).WillOnce(Return(tempRead));
    EXPECT_CALL(*fan_, disable());

    sut_.updateRpm();
}

}   // namespace controller_ut
