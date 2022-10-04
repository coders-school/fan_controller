#pragma once

#include <string>

class ILcdDisplay
{
  public:
    virtual ~ILcdDisplay() = default;

    virtual void print(const std::string& message) const = 0;
};
