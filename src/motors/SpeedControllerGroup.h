#ifndef SPEEDCONTROLLERGROUP_H
#define SPEEDCONTROLLERGROUP_H

#include <Arduino.h>
#include "L298N_Motor.h"

template <size_t numberOfMotors>

class SpeedControllerGroup
{
protected:
    motor::L298N_Motor motors[numberOfMotors];

public:
    SpeedControllerGroup(motor::L298N_Motor* motors);
    void set(double);

    void setMotorDirection(boolean inverted);

    void stopMotors();
};
#endif