#include"SpeedControllerGroup.h"

template<size_t numberOfMotors>
SpeedControllerGroup<numberOfMotors>::SpeedControllerGroup(motor::L298N_Motor* motors)
{
    this->motors = motors;
}
template<size_t numberOfMotors>
void SpeedControllerGroup<numberOfMotors>::set(double speed)
{
    for(motor::L298N_Motor motor : motors)
    {
        motor.set(speed);
    }
}

template<size_t numberOfMotors>
void SpeedControllerGroup<numberOfMotors>::setMotorDirection(boolean inverted)
{
    for(motor::L298N_Motor motor : motors)
    {
        motor.setMotorDirection(inverted);
    }
}
template<size_t numberOfMotors>
void SpeedControllerGroup<numberOfMotors>::stopMotors()
{
    for(motor::L298N_Motor motor : motors)
    {
        motor.stopMotor();
    }
}