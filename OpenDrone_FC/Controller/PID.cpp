/*
 * Copyright (c) OpenDrone, 2018.  All rights reserved.
 * The entire project (including this file) is licensed under the GNU GPL v3.0
 * Purpose: This class is used to calculate the speed of the single motors of the drone
 *
 * 	@author Thomas Brych, Tim Klecka
 * 	@version 0.0.2 27.06.2019
 */
#include "PID.h"

PID *PID::instance = 0;

PID::PID(Orientation *o, PWMMotorTest *p, Barometer *b, Ultrasonic *u)
{
	orientation = o;
	pwm = p;
	barometer = b;
	ultrasonic = u;
}

PID::~PID()
{
}

/**
	This method is used to create/get the instance of the PID-Class (Singleton!)
	@return PID *

	@params Orientation *o, PWMMotorTest *p, Barometer *b, Ultrasonic *u
*/

PID *PID::getInstance(Orientation *o, PWMMotorTest *p, Barometer *b, Ultrasonic *u)
{
	if (instance == 0)
	{
		instance = new PID(o, p, b, u);
	}
	return instance;
}

/**
	This method is used to get the instance of the (already instanced) PID-Class (Singleton!)
	@return PID *
*/
PID *PID::getInstanceCreated()
{
	return instance;
}

/**
	This method meshes the values from the different PIDs and sets the speed of the motors
	@return void
*/
void PID::calcValues()
{
	while (!run) {
		delay(50);
	}

	while (run) {
		calcPid();

		if (throttle + pid_output_height < 1800 && throttle + pid_output_height > 1200) {
			throttle = throttle + pid_output_height;
		}
		esc_1 = throttle - pid_output_pitch + pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 1 (front-right - CCW)
		esc_2 = throttle + pid_output_pitch + pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 2 (rear-right - CW)
		esc_3 = throttle + pid_output_pitch - pid_output_roll - pid_output_yaw;   //Calculate the pulse for esc 3 (rear-left - CCW)
		esc_4 = throttle - pid_output_pitch - pid_output_roll + pid_output_yaw;   //Calculate the pulse for esc 4 (front-left - CW)

		int speedMin = 1100;
		if (esc_1 < speedMin) esc_1 = speedMin;           //Keep the motors running.
		if (esc_2 < speedMin) esc_2 = speedMin;           //Keep the motors running.
		if (esc_3 < speedMin) esc_3 = speedMin;           //Keep the motors running.
		if (esc_4 < speedMin) esc_4 = speedMin;           //Keep the motors running.

		int speedMax = 1900;
		if (esc_1 > speedMax) esc_1 = speedMax;           //Limit the esc-1 pulse to 2500.
		if (esc_2 > speedMax) esc_2 = speedMax;           //Limit the esc-2 pulse to 2500.
		if (esc_3 > speedMax) esc_3 = speedMax;           //Limit the esc-3 pulse to 2500.
		if (esc_4 > speedMax) esc_4 = speedMax;           //Limit the esc-4 pulse to 2500.  
		
		pwm->SetSpeed(1, esc_1);	//Front left
		pwm->SetSpeed(2, esc_2);	//Rear left
		pwm->SetSpeed(3, esc_3);	//Rear right
		pwm->SetSpeed(0, esc_4);	//Front right
		delay(5);
	}

	delay(100);
	if (!stop) {
		calcValues();
	}

	pwm->SetSpeed(16, 0);
}

/**
	This method calculates the values for the different PIDs (Pitch, Roll, Yaw, Throttle)
	@return void
*/
void PID::calcPid() {
	double *ar = orientation->getPitchRoll();

	//std::cout << ar[0] << " " << ar[1] << " " << ar[2] << "\n";

	//Roll calculations
	pid_error_temp = ar[1] - pid_roll_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_roll += pid_i_gain_roll * pid_error_temp;
	if (pid_i_mem_roll > pid_max_roll)pid_i_mem_roll = pid_max_roll;
	else if (pid_i_mem_roll < pid_max_roll * -1)pid_i_mem_roll = pid_max_roll * -1;

	pid_output_roll = pid_p_gain_roll * pid_error_temp + pid_i_mem_roll + pid_d_gain_roll * ((pid_error_temp - pid_last_roll_d_error));
	if (pid_output_roll > pid_max_roll)pid_output_roll = pid_max_roll;
	else if (pid_output_roll < pid_max_roll * -1)pid_output_roll = pid_max_roll * -1;

	pid_last_roll_d_error = pid_error_temp;

	//Pitch calculations
	pid_cur_val = pid_error_temp = ar[0] - pid_pitch_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_pitch += pid_i_gain_pitch * pid_error_temp;
	if (pid_i_mem_pitch > pid_max_pitch)pid_i_mem_pitch = pid_max_pitch;
	else if (pid_i_mem_pitch < pid_max_pitch * -1)pid_i_mem_pitch = pid_max_pitch * -1;

	pid_output_pitch = pid_p_gain_pitch * pid_error_temp + pid_i_mem_pitch + pid_d_gain_pitch * ((pid_error_temp - pid_last_pitch_d_error));
	if (pid_output_pitch > pid_max_pitch)pid_output_pitch = pid_max_pitch;
	else if (pid_output_pitch < pid_max_pitch * -1)pid_output_pitch = pid_max_pitch * -1;

	pid_last_pitch_d_error = pid_error_temp;

	//Yaw calculations
	pid_error_temp = ar[2] - pid_yaw_setpoint;
	if (pid_error_temp != pid_error_temp) {
		calcPid();
	}
	pid_i_mem_yaw += pid_i_gain_yaw * pid_error_temp;
	if (pid_i_mem_yaw > pid_max_yaw)pid_i_mem_yaw = pid_max_yaw;
	else if (pid_i_mem_yaw < pid_max_yaw * -1)pid_i_mem_yaw = pid_max_yaw * -1;

	pid_output_yaw = pid_p_gain_yaw * pid_error_temp + pid_i_mem_yaw + pid_d_gain_yaw * ((pid_error_temp - pid_last_yaw_d_error));
	if (pid_output_yaw > pid_max_yaw)pid_output_yaw = pid_max_yaw;
	else if (pid_output_yaw < pid_max_yaw * -1)pid_output_yaw = pid_max_yaw * -1;

	pid_last_yaw_d_error = pid_error_temp;

	//Throttle calculations
	if (heightControl) {
		double curDistance = ultrasonic->getDistance();
		pid_error_temp = wantedDistane - curDistance;

		pid_output_height = pid_p_gain_height * pid_error_temp + pid_d_gain_height * ((pid_error_temp - pid_last_height_error));
		pid_last_height_error = pid_error_temp;
	}
	else {
		pid_output_height = 0.0;
	}

}

/**
	This method is used to set the P-Part of the Pitch/Roll PIDs (only for testing!)
	@return void

	@params float curP
*/
void PID::setP(float curP) {
	/*if (curP >= 0) {
		pid_p_gain_roll = curP;
		pid_p_gain_pitch = curP;
	}*/
}

/**
	This method is used to set the I-Part of the Pitch/Roll PIDs (only for testing!)
	@return void

	@params float curI
*/
void PID::setI(float curI) {
	/*if (curI >= 0) {
		pid_i_gain_roll = curI;
		pid_i_gain_pitch = curI;
	}*/
}

/**
	This method is used to set the D-Part of the Pitch/Roll PIDs (only for testing!)
	@return void

	@params float curD
*/
void PID::setD(float curD) {
	/*if (curD >= 0) {
		pid_d_gain_roll = curD;
		pid_d_gain_pitch = curD;
	}*/
}

/**
	This method is used to start/stop the PIDs
	@return void

	@params bool curRun
*/
void PID::setRun(bool curRun) {
	run = curRun;
}

/**
	This method is used to set the throttle (called from the Modbus.cpp)
	@return void

	@params float curThrottle
*/
void PID::setThrottle(float curThrottle) {
	if (curThrottle > 1100 && curThrottle < 1700) {
		throttle = curThrottle;
	}
}

/**
	This method is used to set the wanted pitch-value (fly forward/backward)
	@return void

	@params int curPitchSetpoint
*/
void PID::setPitchSetpoint(int curPitchSetpoint) {
	if (curPitchSetpoint >= 1000 && curPitchSetpoint <= 2000) {
		int diff = 0;
		if (curPitchSetpoint > 1480 && curPitchSetpoint < 1520) {
			pid_pitch_setpoint = 0;
		} else if (curPitchSetpoint < 1480) {
			diff = (curPitchSetpoint - 1480)*-1;
		} else if (curPitchSetpoint > 1520) {
			diff = (curPitchSetpoint - 1520)*-1;
		}
		pid_pitch_setpoint = diff * factorControl;
	}
}

/**
	This method is used to set the wanted roll-value (fly right/left)
	@return void

	@params int curRollSetpoint
*/
void PID::setRollSetpoint(int curRollSetpoint) {
	int setPoint = curRollSetpoint;
	if (setPoint >= 1000 && setPoint <= 2000) {
		int diff = 0;
		if (setPoint > 1480 && setPoint < 1520) {
			pid_roll_setpoint = 0;
		}
		else if (setPoint < 1480) {
			diff = (setPoint - 1480)*-1;
		}
		else if (setPoint > 1520) {
			diff = (setPoint - 1520)*-1;
		}
		pid_roll_setpoint = diff * factorControl;
	}
}

/**
	This method is used to set the wanted yaw-value (rotate the drone)
	@return void

	@params int curYawSetpoint
*/
void PID::setYawSetpoint(int curYawSetpoint) {
	/*int setPoint = curYawSetpoint;
	if (setPoint >= 1000 && setPoint <= 2000) {
		int diff = 0;
		if (setPoint > 1480 && setPoint < 1520) {
			pid_yaw_setpoint = 0;
		}
		else if (setPoint < 1480) {
			diff = (setPoint - 1480)*-1;
		}
		else if (setPoint > 1520) {
			diff = (setPoint - 1520)*-1;
		}
		pid_yaw_setpoint = diff / 5;
	}*/
}

/**
	This method is used to arm the motors
	@return void
*/
void PID::armMotor() {
	pwm->ExitMotor();
	pwm->ArmMotor();
}

/**
	This method is used to interrupt the PIDs
	@return void
*/
void PID::interruptPid() {
	run = false;
	stop = true;
}

/**
	This method returns the speed of all motors (used for logging into the database)
	@return void
*/
int *PID::getThrottles() {
	static int ar[5];
	ar[0] = esc_1;
	ar[1] = esc_2;
	ar[2] = esc_3;
	ar[3] = esc_4;
	ar[4] = throttle;
	return ar;
}

/**
	This method returns the output of the pid-output (used for logging into the database)
	@return void
*/
float *PID::getPIDVals() {
	static float ar[3];
	ar[0] = pid_output_pitch;
	ar[1] = pid_cur_val;
	ar[2] = pid_i_mem_pitch;
	return ar;
}

/**
	This method is used to enable/disable the height control of the drone
	@return void
*/
void PID::updateHeightControl() {
	heightControl = !heightControl;
}

/**
	This method checks if the PID has already been initialized
	@return bool
*/
bool PID::isInit() {
	if (orientation != NULL) 
		return true;
	else 
		return false;
}

/**
	This method returns the Orientation object
	@return Orientation *
*/
Orientation *PID::getOrientatin() {
	return orientation;
}

/**
	This method returns the PWMMotorTest object
	@return PWMMotorTest *
*/
PWMMotorTest *PID::getPwmMotorTest() {
	return pwm;
}
