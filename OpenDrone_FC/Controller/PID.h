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
	Orientation *getOrientation();

private:
	PID(Orientation *o, PWMMotorTest *p);
	PWMMotorTest *pwm;
	Orientation *orientation;

	long time;
	int esc_1, esc_2, esc_3, esc_4;
	int throttle;

	float pid_p_gain_roll = 0.69;               //Gain setting for the roll P-controller 0.8
	float pid_i_gain_roll = 0.021;              //Gain setting for the roll I-controller 0.0175
	float pid_d_gain_roll = 17.0;              //Gain setting for the roll D-controller 10.0
	int pid_max_roll = 1500;                    //Maximum output of the PID-controller (+/-)

	float pid_p_gain_pitch = pid_p_gain_roll;  //Gain setting for the pitch P-controller.
	float pid_i_gain_pitch = pid_i_gain_roll;  //Gain setting for the pitch I-controller.
	float pid_d_gain_pitch = pid_d_gain_roll;  //Gain setting for the pitch D-controller.
	int pid_max_pitch = pid_max_roll;          //Maximum output of the PID-controller (+/-)

	float pid_p_gain_yaw = 4.0;                //Gain setting for the pitch P-controller. //4.0
	float pid_i_gain_yaw = 0.02;               //Gain setting for the pitch I-controller. //0.02
	float pid_d_gain_yaw = 0.00;                //Gain setting for the pitch D-controller. 0.00
	int pid_max_yaw = 1500;                     //Maximum output of the PID-controller (+/-)

	bool auto_level = true;                 //Auto level on (true) or off (false)


	float pid_error_temp;
	float pid_i_mem_roll = 0, pid_roll_setpoint, pid_output_roll, pid_last_roll_d_error = 0;
	float pid_i_mem_pitch = 0, pid_pitch_setpoint,pid_output_pitch, pid_last_pitch_d_error = 0;
	float pid_i_mem_yaw = 0, pid_yaw_setpoint, pid_output_yaw, pid_last_yaw_d_error = 0;
	
	void calcPid();

};

