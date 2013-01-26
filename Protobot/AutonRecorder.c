#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorRight,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     RightRear,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     RightFront,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     LeftRear,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     LeftFront,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring it up!"

	Written by Joe Quigley
	A "recorder" of driver movements that saves in "steps" to a file. This file can then be
	read by the autonomus program.

	Revisions:
n		Rev		Date		Notes
1		0.0		1/19/13	First version
2		0.1		1/23/13	Fixed a few things and made sure encoder vals were contrained to ints
*/

#include "stdbot_d.h"
#include "JoystickDriver.c"

int nSteps = 0;
bool cont = true;

int mscal(int mval)	{
	if(abs(mval) > 10)	{
		return (Map(mval, -127, 127, -100, 100));
	}
	else	{
		return 0;
	}
}

task main()
{
	initWriteMode("auto.txt", 1024, true);

	while(cont)	{
		motor[leftRear	] = mscal(joystick.joy1_y1);
		motor[leftFront	] = mscal(joystick.joy1_y1);
		motor[rightRear	] = mscal(joystick.joy1_y2);
		motor[rightFront] = mscal(joystick.joy1_y2);
		motor[ScissorLeft] = mscal(joystick.joy2_y2);
		motor[ScissorRight] = mscal(joystick.joy2_y2);

		if(joy1Btn(1) && (getElapsed() == 0))	{
			PlayTone(523, 15);
			beginNewTimer(1000);
			writeInt(nSteps);
			writeInt(nMotorEncoder[leftFront	]/1000);
			writeInt(nMotorEncoder[leftRear		]/1000);
			writeInt(nMotorEncoder[rightFront	]/1000);
			writeInt(nMotorEncoder[rightRear	]/1000);
			writeInt(nMotorEncoder[ScissorLeft]/1000);

			nSteps++;
		}

		if(joy1Btn(2))	{
			nMotorEncoder[leftFront] = 0;
			nMotorEncoder[leftRear] = 0;
			nMotorEncoder[rightFront] = 0;
			nMotorEncoder[rightRear] = 0;
			nMotorEncoder[ScissorLeft] = 0;
			nMotorEncoder[ScissorRight] = 0;
		}

		if(joy1Btn(3))	{
			cont = false;
			PlayTone(300, 15);
			closeActiveFile();
		}
	}
}
