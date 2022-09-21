#pragma once

class Fan
{
  public:
    void setSpeed(int newRpm);
    int getSpeed();
    bool disable();
    bool enable();

  private:
    int rpm_ = 0;
};