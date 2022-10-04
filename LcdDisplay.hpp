#pragma once

#include "ILcdDisplay.hpp"

class LcdDisplay : public ILcdDisplay
{
  public:
    void print(const std::string& message) const override;
};
