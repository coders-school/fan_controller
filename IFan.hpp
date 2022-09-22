#pragma once

class IFan
{
  public:
    virtual ~IFan() = default;
    virtual void setSpeed(int newRpm) = 0;
    virtual int getSpeed() const = 0;
    virtual bool disable() = 0;
    virtual bool enable() = 0;
};