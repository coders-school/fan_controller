#pragma once

#include "ILcdDisplay.hpp"

#include "gmock/gmock.h"

class LcdDisplayMock : public ILcdDisplay
{
  public:
    MOCK_METHOD(void, print, (const std::string& ), (const, override));
};
