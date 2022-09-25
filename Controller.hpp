#pragma once
// TODO: REMOVE
#include "Fan.hpp"
// TODO: REMOVE
// #include "SlowThermometer/SlowThermometer.hpp"

#include <memory>

class LcdDisplay;
class IThermometer;
class Controller
{
  public:
    Controller(const IThermometer& thermometer,
               IFan& fan,
               double targetTemperature,
               double tolerance);
    virtual ~Controller() = default;
    void updateRpm();

  protected:
    const IThermometer& thermometer_;
    IFan& fan_;
    const double fanStartTemperature_;
    const double increaseSpeedTemperature_;
};
