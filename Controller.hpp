#pragma once

#include <memory>

class IFan;
class IThermometer;
class Controller
{
  public:
    Controller(const IThermometer& thermometer,
               IFan& fan,
               double targetTemperature,
               double tolerance);
    virtual ~Controller() = default;
    void updateRpm() const;

  protected:
    int calcDesiredFanSpeed(double temperature) const;
    void disableFan(int currentRpm) const;
    void setFanToNominalSpeed(int currentRpm) const;
    void setFanToIncreasedSpeed(int currentRpm, double temperature) const;

    const IThermometer& thermometer_;
    IFan& fan_;
    const double fanStartTemperature_;
    const double nominalFanSpeedUpperThreshold_;
    const int fanNominalSpeed_ = 1000;
    const int fanMaxSpeed_ = 3000;
    const double tempDiffForOneRpm_ = 0.001;
};
