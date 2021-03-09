#include <Arduino.h>
#include "L298N_Motor.h"

motor::L298N_Motor::L298N_Motor(int enable, int inA, int inB)
{
    this->enable = enable;
    pinMode(enable, OUTPUT);

    this->inA = inA;
    pinMode(inA, OUTPUT);

    this->inB = inB;
    pinMode(inB, OUTPUT);
}

void motor::L298N_Motor::setMotorDirection(boolean inverted)
{
    if (inverted)
    {
        digitalWrite(inA, HIGH);
        digitalWrite(inB, LOW);
    }
    else if (!inverted)
    {
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
    }
}

void motor::L298N_Motor::set(double speed)
{
    double output = map(speed, -1, 1, 0, 1023);
    if(speed > 0){
        setMotorDirection(true);
    }    
    else if(speed < 0){
        setMotorDirection(false);
    }
    analogWrite(enable, output);
}

void motor::L298N_Motor::stopMotor(){
    analogWrite(enable, 0);
}
