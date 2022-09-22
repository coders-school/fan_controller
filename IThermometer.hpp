#pragma once

class IThermometer
{
  public:
    virtual ~IThermometer() = default;
    virtual double getTemperature() const = 0;
};