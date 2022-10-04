#pragma once

#include "IFan.hpp"

class Fan : public IFan
{
  public:
    void setSpeed(int newRpm) override;
    int getSpeed() const override;
    bool disable() override;
    bool enable() override;

  private:
    static constexpr double nominalRpmLimit = 1000;
    static constexpr double higherRpmLimit = 3000;
    int rpm_ = 0;
};
