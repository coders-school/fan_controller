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
    , nominalFanSpeedUpperThreshold_(targetTemperature + tolerance)
{ }

void Controller::updateRpm()
{
    const double tempRead = thermometer_.getTemperature();
    if (tempRead < fanStartTemperature_) {
        fan_.disable();
    }
    else if (tempRead >= fanStartTemperature_ && tempRead <= nominalFanSpeedUpperThreshold_) {
        fan_.enable();
    }
}