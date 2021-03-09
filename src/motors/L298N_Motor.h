#ifndef L298N_MOTOR_H
#define L298N_MOTOR_H

namespace motor
{
    class L298N_Motor
    {
        protected:
        int enable;
        int inA;
        int inB;
        boolean isInverted;

        public:
        L298N_Motor() = default;
        
        L298N_Motor(int enable, int inA, int inB);

        void set(double speed);

        void stopMotor();

        void setMotorDirection(boolean inverted);
    };
}
    

#endif