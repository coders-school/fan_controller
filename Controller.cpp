#include "Controller.hpp"

#include "IThermometer.hpp"

Controller::Controller(const IThermometer& thermometer,
                       Fan fan,
                       double,
                       double,
                       std::shared_ptr<LcdDisplay> display)
{ }

void Controller::updateRpm()
{ }

void Controller::displayInfo()
{ }
