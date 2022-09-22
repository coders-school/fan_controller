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
               Fan fan,
               double,
               double,
               std::shared_ptr<LcdDisplay> display);
    void updateRpm();
    void displayInfo();
};
