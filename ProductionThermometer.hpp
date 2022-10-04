#pragma once

#include "IThermometer.hpp"

#include <memory>

class SlowThermometer;

class ProductionThermometer : public IThermometer
{
  public:
    ProductionThermometer(const SlowThermometer& vendorThermometer);
    double getTemperature() const override;

  private:
    const SlowThermometer& thermometer_;
};
