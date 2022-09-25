// TODO: VERIFY includes
#include "Controller.hpp"

#include "IFan.hpp"
#include "IThermometer.hpp"

Controller::Controller(const IThermometer& thermometer,
                       IFan& fan,
                       double targetTemperature,
                       double tolerance)
    : thermometer_(thermometer)
    , fan_(fan)
    , fanStartTemperature_(targetTemperature - tolerance)
    , increaseSpeedTemperature_(targetTemperature + tolerance)
{ }

void Controller::updateRpm()
{
    const double tempRead = thermometer_.getTemperature();
    if (tempRead < fanStartTemperature_) {
        fan_.disable();
    }
}