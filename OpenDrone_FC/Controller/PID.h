#include "Orientation.h"
#include "../Motor/PWMMotorTest.h"
#include "wiringPi.h"
#include "iostream"

#pragma once
class PID
{
public:
	PID(Orientation *o, PWMMotorTest *p);
	~PID();

	void calcValues();
private:
	PWMMotorTest *pwm;
	Orientation *orientation;

	long time;
	int esc_1, esc_2, esc_3, esc_4;
	int throttle;

	float pid_p_gain_roll = 0.65;               //Gain setting for the roll P-controller 1.3
	float pid_i_gain_roll = 0.2;              //Gain setting for the roll I-controller 0.4
	float pid_d_gain_roll = 9.0;              //Gain setting for the roll D-controller 18.0
	int pid_max_roll = 1000;                    //Maximum output of the PID-controller (+/-)

	float pid_p_gain_pitch = pid_p_gain_roll;  //Gain setting for the pitch P-controller.
	float pid_i_gain_pitch = pid_i_gain_roll;  //Gain setting for the pitch I-controller.
	float pid_d_gain_pitch = pid_d_gain_roll;  //Gain setting for the pitch D-controller.
	int pid_max_pitch = pid_max_roll;          //Maximum output of the PID-controller (+/-)

	float pid_p_gain_yaw = 2.0;                //Gain setting for the pitch P-controller. //4.0
	float pid_i_gain_yaw = 0.01;               //Gain setting for the pitch I-controller. //0.02
	float pid_d_gain_yaw = 0.00;                //Gain setting for the pitch D-controller. 0.00
	int pid_max_yaw = 1000;                     //Maximum output of the PID-controller (+/-)

	bool auto_level = true;                 //Auto level on (true) or off (false)


	float pid_error_temp;
	float pid_i_mem_roll = 0, pid_roll_setpoint, pid_output_roll, pid_last_roll_d_error = 0;
	float pid_i_mem_pitch = 0, pid_pitch_setpoint,pid_output_pitch, pid_last_pitch_d_error = 0;
	float pid_i_mem_yaw = 0, pid_yaw_setpoint, pid_output_yaw, pid_last_yaw_d_error = 0;
	
	void calcPid();
};

