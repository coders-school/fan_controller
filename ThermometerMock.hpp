#include "IThermometer.hpp"

#include "gmock/gmock.h"

class ThermometerMock : public IThermometer
{
  public:
    MOCK_METHOD(double, getTemperature, (), (const, override));
};
