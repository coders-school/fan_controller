#include "ControllerWithDisplay.hpp"

#include "IFan.hpp"
#include "ILcdDisplay.hpp"
#include "IThermometer.hpp"

ControllerWithDisplay::ControllerWithDisplay(const IThermometer& thermometer,
                                             IFan& fan,
                                             double targetTemperature,
                                             double tolerance,
                                             const ILcdDisplay& display)
    : Controller(thermometer, fan, targetTemperature, tolerance)
    , display_(display)
{
}

std::string ControllerWithDisplay::prepareMessage()
{
    std::string message;
    message += "Temperature: " + std::to_string(thermometer_.getTemperature()) + "\n"
        + "Fan RPM: " + std::to_string(fan_.getSpeed()) + "\n";

    return message;
}

void ControllerWithDisplay::displayInfo()
{
    display_.print(prepareMessage());
}
