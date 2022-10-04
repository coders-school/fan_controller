#include "Fan.hpp"

#include "gmock/gmock.h"

class FanMock : public IFan
{
  public:
    MOCK_METHOD(void, setSpeed, (int newRpm), (override));
    MOCK_METHOD(int, getSpeed, (), (const, override));
    MOCK_METHOD(bool, disable, (), (override));
    MOCK_METHOD(bool, enable, (), (override));
};
