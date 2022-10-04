#include "Controller.hpp"

#include "IFan.hpp"
#include "IThermometer.hpp"

#include <cmath>
#include <stdexcept>

Controller::Controller(const IThermometer& thermometer,
                       IFan& fan,
                       double targetTemperature,
                       double tolerance)
    : thermometer_(thermometer)
    , fan_(fan)
    , fanStartTemperature_(targetTemperature - tolerance)
    , nominalFanSpeedUpperThreshold_(targetTemperature + tolerance)
{ }

void Controller::updateRpm() const
{
    const double currentRpm = fan_.getSpeed();
    const double tempRead = thermometer_.getTemperature();

    if (tempRead < fanStartTemperature_) {
        disableFan(currentRpm);
    }
    else if (tempRead >= fanStartTemperature_
             && tempRead <= nominalFanSpeedUpperThreshold_) {
        setFanToNominalSpeed(currentRpm);
    }
    else {
        setFanToIncreasedSpeed(currentRpm, tempRead);
    }
}

int Controller::calcDesiredFanSpeed(double temperature) const
{
    if (temperature < nominalFanSpeedUpperThreshold_) {
        throw std::invalid_argument("attempt to calc Desired fan speed for temp lower than"
                                    " required for increasing speed ");
    }

    const double tempDiff = temperature - nominalFanSpeedUpperThreshold_;
    int rpmIncrease = static_cast<int>(std::round(tempDiff / tempDiffForOneRpm_));
    return fanNominalSpeed_ + rpmIncrease;
}

void Controller::setFanToNominalSpeed(int currentRpm) const
{
    if (currentRpm == 0) {
        fan_.enable();
        return;
    }
    else if (currentRpm == fanNominalSpeed_) {
        return;
    }

    fan_.setSpeed(fanNominalSpeed_);
}

void Controller::disableFan(int currentRpm) const
{
    if (currentRpm != 0) {

        fan_.disable();
    }
}

void Controller::setFanToIncreasedSpeed(int currentRpm, double temperature) const
{
    int calculatedRPM = calcDesiredFanSpeed(temperature);
    int desiredRPM = calculatedRPM >= fanMaxSpeed_ ? fanMaxSpeed_
                                                   : calculatedRPM;
    if (desiredRPM != currentRpm) {
        fan_.setSpeed(desiredRPM);
    }
}
