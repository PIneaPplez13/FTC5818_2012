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

#include "stdbot_d.h"

#define nSTEPS 2

bool cont = true;
int mtr[6];
int *mref = mtr;

task main()
{
	for(; mref < mtr+sizeof(mtr); mref++)	{
		*mref = 0;
	}

	initReadMode("auto.txt", 1024);

	nMotorEncoder[leftFront	] = 0;
	nMotorEncoder[leftRear	] = 0;
	nMotorEncoder[rightFront] = 0;
	nMotorEncoder[rightRear	] = 0;
	nMotorEncoder[ScissorLeft] = 0;
	nMotorEncoder[ScissorRight] = 0;

	for(int i = 0; i < nSTEPS; i++)	{
		PlayTone(523, 150);
		nextInt();
		mtr[0] = nextInt() * 1000;
		mtr[1] = nextInt() * 1000;
		mtr[2] = nextInt() * 1000;
		mtr[3] = nextInt() * 1000;
		mtr[4] = nextInt() * 1000;
		mtr[5] = nextInt() * 1000;

		motor[leftFront	] = 100 * sgn(mtr[0]);
		motor[leftRear	] = 100 * sgn(mtr[1]);
		motor[rightFront] = 100 * sgn(mtr[2]);
		motor[rightRear	] = 100 * sgn(mtr[3]);/*
		motor[ScissorLeft] = 100 * sgn(mtr[4]);
		motor[ScissorRight] = 100 * sgn(mtr[5]);*/

		while(cont)	{
			if((mtr[0] != 0) && (abs(nMotorEncoder[leftFront]) > abs(mtr[0])))	{
				motor[leftFront] = 0;
				nMotorEncoder[leftFront] = 0;
				mtr[0] = 0;
			}
			if((mtr[1] != 0) && (abs(nMotorEncoder[leftRear]) > abs(mtr[1])))	{
				motor[leftRear] = 0;
				nMotorEncoder[leftRear] = 0;
				mtr[1] = 0;
			}
			if((mtr[2] != 0) && (abs(nMotorEncoder[rightFront]) > abs(mtr[2])))	{
				motor[rightFront] = 0;
				nMotorEncoder[rightFront] = 0;
				mtr[2] = 0;
			}
			if((mtr[3] != 0) && (abs(nMotorEncoder[rightRear]) > abs(mtr[3])))	{
				motor[rightRear] = 0;
				nMotorEncoder[rightRear] = 0;
				mtr[3] = 0;
			}/*
			if((mtr[4] != 0) && (abs(nMotorEncoder[ScissorLeft]) > abs(mtr[4])))	{
				motor[ScissorLeft] = 0;
				nMotorEncoder[ScissorLeft] = 0;
				mtr[4] = 0;
			}
			if((mtr[5] != 0) && (abs(nMotorEncoder[ScissorRight]) > abs(mtr[5])))	{
				motor[ScissorRight] = 0;
				nMotorEncoder[ScissorRight] = 0;
				mtr[5] = 0;
			}*/

			if((mtr[0] == 0) && (mtr[1] == 0) && (mtr[2] == 0) && (mtr[3] == 0) && (mtr[4] == 0) && (mtr[5] == 0))	{
				cont = false;
			}
		}

		cont = true;

		motor[leftFront	] = 0;
		motor[leftRear	] = 0;
		motor[rightFront] = 0;
		motor[rightRear	] = 0;
		motor[ScissorLeft] = 0;
		motor[ScissorRight] = 0;

		wait1Msec(100);
	}
}
