#include "Controller.hpp"
#include "SlowThermometer/SlowThermometer.hpp"
#include "Fan.hpp"
#include "LcdDisplay.hpp"
#include <memory>
#include <iostream>

int main()
try {
    SlowThermometer t;
    Fan f;
    const double targetTemperature = 36.6;
    const double tolerance = .5;
    Controller oldController{t, f, targetTemperature, tolerance, nullptr};
    oldController.updateRpm();
    oldController.displayInfo();

    Controller newController{t, f, targetTemperature, tolerance, std::shared_ptr<LcdDisplay>()};
    newController.updateRpm();
    newController.displayInfo();

    return 0;
} catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return -1;
}

