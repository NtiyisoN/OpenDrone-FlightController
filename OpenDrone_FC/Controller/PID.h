#include "Orientation.h"
#include "../Motor/PWMMotorTest.h"
#include "wiringPi.h"
#include "iostream"

#pragma once
class PID
{
public:
	~PID();
	static PID *getInstance(Orientation *o, PWMMotorTest *p);
	static PID *instance;
	void calcValues();

	void setP(float curP);
	void setI(float curI);
	void setD(float curD);
	void setThrottle(float curThrottle);
	void setRun(bool curRun);
	void armMotor();
	Orientation *getOrientation();

private:
	PID(Orientation *o, PWMMotorTest *p);
	PWMMotorTest *pwm = NULL;
	Orientation *orientation = NULL;

	long time;
	int esc_1, esc_2, esc_3, esc_4;
	int throttle = 1500;
	bool run = false;

	bool auto_level = true;                 //Auto level on (true) or off (false)


	float pid_error_temp;
	float pid_i_mem_roll = 0, pid_roll_setpoint, pid_output_roll, pid_last_roll_d_error = 0;
	float pid_i_mem_pitch = 0, pid_pitch_setpoint,pid_output_pitch, pid_last_pitch_d_error = 0;
	float pid_i_mem_yaw = 0, pid_yaw_setpoint, pid_output_yaw, pid_last_yaw_d_error = 0;
	
	void calcPid();

};

