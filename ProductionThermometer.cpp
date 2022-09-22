#include "ProductionThermometer.hpp"
#include "SlowThermometer/SlowThermometer.hpp"

ProductionThermometer::ProductionThermometer(const SlowThermometer& vendorThermometer)
    : thermometer_(vendorThermometer)
{
}

double ProductionThermometer::getTemperature() const
{
    return thermometer_.getTemperature();
}