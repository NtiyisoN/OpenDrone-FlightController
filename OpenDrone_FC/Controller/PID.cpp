/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: TODO
 *
 * 	@author Thomas Brych
 * 	@version 0.0.1 14.02.2019
 */
#include "PID.h"

PID *PID::instance = 0;

PID *PID::getInstance(Orientation *o, PWMMotorTest *p)
{
	if (instance == 0)
	{
		instance = new PID(o, p);
	}
	return instance;
}

PID::PID(Orientation *o, PWMMotorTest *p)
{
	orientation = o;
	pwm = p;
}

void PID::calcValues()
{
	std::cout << "Start";

	while (!run) {
		delay(50);
	}
	while (run) {
		calcPid();

		esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 1 (front-right - CCW)
		esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 2 (rear-right - CW)
		esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 3 (rear-left - CCW)
		esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 4 (front-left - CW)

		int speedMin = 1200;
		if (esc_1 < speedMin) esc_1 = speedMin;           //Keep the motors running.
		if (esc_2 < speedMin) esc_2 = speedMin;           //Keep the motors running.
		if (esc_3 < speedMin) esc_3 = speedMin;           //Keep the motors running.
		if (esc_4 < speedMin) esc_4 = speedMin;           //Keep the motors running.

		int speedMax = 2500;
		if (esc_1 > speedMax) esc_1 = speedMax;           //Limit the esc-1 pulse to 2500.
		if (esc_2 > speedMax) esc_2 = speedMax;           //Limit the esc-2 pulse to 2500.
		if (esc_3 > speedMax) esc_3 = speedMax;           //Limit the esc-3 pulse to 2500.
		if (esc_4 > speedMax) esc_4 = speedMax;           //Limit the esc-4 pulse to 2500.  
		
																			   
		/*pwm->SetSpeed(0, esc_1);
		pwm->SetSpeed(1, esc_2);
		pwm->SetSpeed(2, esc_1);
		pwm->SetSpeed(3, esc_4);*/

		/*pwm->SetSpeed(3, esc_1);
		pwm->SetSpeed(3+4, esc_1);
		pwm->SetSpeed(2, esc_2);
		pwm->SetSpeed(2 + 4, esc_2);*/

		//std::cout << esc_1 << " " << esc_2 << " " << esc_3 << " " << esc_4 << "\n";
	}

	std::cout << "Stop";
	std::cout.flush();
	pwm->SetSpeed(16, 0);

	//Change this
	exit(1);
}

void PID::calcPid() {
	//std::cout << pid_p_gain_roll << " " << pid_i_gain_roll << " " << pid_d_gain_roll << "\n";
	double *ar = orientation->getPitchRoll();
	ar[1] = 0;
	ar[2] = 0;
	/*double ar[3];
	ar[0] = 50;
	ar[1] = 0;
	ar[2] = 0;*/

	//Roll calculations
	pid_error_temp = ar[1] - pid_roll_setpoint;
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	if (pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
	else if (pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

	pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * (pid_error_temp - pid_last_roll_d_error);
	if (pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
	else if (pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

	pid_last_roll_d_error = pid_error_temp;

	//Pitch calculations
	pid_error_temp = ar[0] - pid_pitch_setpoint;
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	if (pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
	else if (pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;

	pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * (pid_error_temp - pid_last_pitch_d_error);
	if (pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
	else if (pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

	pid_last_pitch_d_error = pid_error_temp;

	//Yaw calculations
	pid_error_temp = ar[2] - pid_yaw_setpoint;
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
	if (pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
	else if (pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

	pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * (pid_error_temp - pid_last_yaw_d_error);
	if (pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
	else if (pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

	pid_last_yaw_d_error = pid_error_temp;
	//std::cout << pid_output_pitch << " " << pid_output_roll << " " << pid_output_yaw << "\n";
	//std::cout.flush();
}

void PID::setP(float curP) {
	if (curP != 0) {
		pid_p_gain_roll = curP;
		pid_p_gain_pitch = curP;
	}
}

void PID::setI(float curI) {
	if (curI != 0) {
		pid_i_gain_roll = curI;
		pid_i_gain_pitch = curI;
	}
}

void PID::setD(float curD) {
	if (curD != 0) {
		pid_d_gain_roll = curD;
		pid_d_gain_pitch = curD;
	}
}

void PID::setRun(bool curRun) {
	run = curRun;
}

void PID::setThrottle(float curThrottle) {
	if (curThrottle >= 0 && curThrottle <= 100) 
	{
		throttle = 1500 + (curThrottle * 5);		//1500 - 2000
	}
	else if (curThrottle < 0 && curThrottle >= -100) 
	{
		throttle = 1500 + (curThrottle * 5);		//1000 - 1500
	}

	pwm->SetSpeed(0, throttle);
	pwm->SetSpeed(8, throttle);
}

void PID::setPitchSetpoint(int curPitchSetpoint) {
	pwm->SetSpeed(1, curPitchSetpoint);
	pwm->SetSpeed(9, curPitchSetpoint);


	//The PID set point in degrees per second is determined by the pitch receiver input.
	//In the case of deviding by 3 the max pitch rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
	//pid_pitch_setpoint = 0;
	/*//We need a little dead band of 16us for better results.
	if (receiver_input_channel_2 > 1508)pid_pitch_setpoint = receiver_input_channel_2 - 1508;
	else if (receiver_input_channel_2 < 1492)pid_pitch_setpoint = receiver_input_channel_2 - 1492;

	pid_pitch_setpoint -= pitch_level_adjust;                                  //Subtract the angle correction from the standardized receiver pitch input value.
	pid_pitch_setpoint /= 3.0;   */                                              //Divide the setpoint for the PID pitch controller by 3 to get angles in degrees.
}

void PID::setRollSetpoint(int curRollSetpoint) {
	pwm->SetSpeed(2, curRollSetpoint);
	pwm->SetSpeed(10, curRollSetpoint);


	//The PID set point in degr1ees per second is determined by the roll receiver input.
	//In the case of deviding by 3 the max roll rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
	//pid_roll_setpoint = 0;
	//We need a little dead band of 16us for better results.
	/*if (receiver_input_channel_1 > 1508)pid_roll_setpoint = receiver_input_channel_1 - 1508;
	else if (receiver_input_channel_1 < 1492)pid_roll_setpoint = receiver_input_channel_1 - 1492;

	pid_roll_setpoint -= roll_level_adjust;										//Subtract the angle correction from the standardized receiver roll input value.
	pid_roll_setpoint /= 3.0;  */                                               //Divide the setpoint for the PID roll controller by 3 to get angles in degrees.

}

void PID::setYawSetpoint(int curYawSetpoint) {
	pwm->SetSpeed(3, curYawSetpoint);
	pwm->SetSpeed(11, curYawSetpoint);

	//The PID set point in degrees per second is determined by the yaw receiver input.
	//In the case of deviding by 3 the max yaw rate is aprox 164 degrees per second ( (500-8)/3 = 164d/s ).
	//pid_yaw_setpoint = 0;
	//We need a little dead band of 16us for better results.
	/*if (receiver_input_channel_3 > 1050) { //Do not yaw when turning off the motors.
		if (receiver_input_channel_4 > 1508)pid_yaw_setpoint = (receiver_input_channel_4 - 1508) / 3.0;
		else if (receiver_input_channel_4 < 1492)pid_yaw_setpoint = (receiver_input_channel_4 - 1492) / 3.0;
	}*/

}

void PID::armMotor() {
	pwm->ExitMotor();
	pwm->ArmMotor();
}

bool PID::isInit() {
	if (orientation != NULL) 
	{
		return true;
	}
	else 
	{
		return false;
	}
}

PID::~PID()
{
}
