#pragma once

class Fan
{
  public:
    void setSpeed(int newRpm);
    int getSpeed() const;
    bool disable();
    bool enable();

  private:
    static constexpr double lowerRpmLimit = 1000;
    static constexpr double higherRpmLimit = 3000;
    int rpm_ = 0;
};