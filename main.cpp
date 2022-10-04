#include "ControllerWithDisplay.hpp"
#include "Fan.hpp"
#include "LcdDisplay.hpp"
#include "ProductionThermometer.hpp"
#include "SlowThermometer/SlowThermometer.hpp"

#include <iostream>
#include <memory>

int main()
try {
    SlowThermometer vendorThermometer;
    ProductionThermometer thermometer(vendorThermometer);

    Fan fan;
    const double targetTemperature = 36.6;
    const double tolerance = .5;

    Controller oldController { thermometer, fan, targetTemperature, tolerance };

    oldController.updateRpm();

    LcdDisplay display;
    ControllerWithDisplay newController { thermometer, fan, targetTemperature, tolerance, display };
    newController.updateRpm();
    newController.displayInfo();

    return 0;
}
catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return -1;
}
