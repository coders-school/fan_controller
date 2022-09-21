#include "Fan.hpp"

#include <cmath>
#include <stdexcept>

void Fan::setSpeed(int newRpm)
{
    if ((newRpm < 1000 and newRpm != 0) or newRpm > 3000) {
        throw std::invalid_argument("Invalid speed");
    }
    auto difference = std::abs(newRpm - rpm_);
    for (auto i = 0; i < difference; ++i) {
        if (newRpm - rpm_ > 0) {
            rpm_++;
        }
        else {
            rpm_--;
        }
    }
}

int Fan::getSpeed()
{
    return rpm_;
}

bool Fan::disable()
{
    rpm_ = 0;
    return true;
}

bool Fan::enable()
{
    rpm_ = 1000;
    return true;
}
