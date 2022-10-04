#pragma once
#include "Controller.hpp"

#include <memory>

class ILcdDisplay;
class IFan;
class LcdDisplay;
class IThermometer;

class ControllerWithDisplay : public Controller
{
  public:
    ControllerWithDisplay(const IThermometer& thermometer,
                          IFan& fan,
                          double targetTemperature,
                          double tolerance,
                          const ILcdDisplay& display);

    void displayInfo();

  private:
    std::string prepareMessage();

    const ILcdDisplay& display_;
};
