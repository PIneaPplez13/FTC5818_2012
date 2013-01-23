#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorRight,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     leftRear,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     rightFront,    tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

#include "stdbot_d.h"

bool cont = true;
int mtr[4];
int *mref = mtr;

task main()
{
	for(; mref < mtr+4; mref++)	{
		*mref = 0;
	}

	initReadMode("auto.txt", 1024);

	nMotorEncoder[leftFront] = 0;
	nMotorEncoder[leftRear] = 0;
	nMotorEncoder[rightFront] = 0;
	nMotorEncoder[rightRear] = 0;

	for(int i = 0; i < 10; i++)	{
		PlayTone(523, 15);
		nextInt();
		mtr[0] = nextInt() * 1000;
		mtr[1] = nextInt() * 1000;
		mtr[2] = nextInt() * 1000;
		mtr[3] = nextInt() * 1000;

		motor[leftFront	] = 100;
		motor[leftRear	] = 100;
		motor[rightFront] = 100;
		motor[rightRear	] = 100;

		while(cont)	{
			if((mtr[0] != 0) && (nMotorEncoder[leftFront] > mtr[0]))	{
				motor[leftFront] = 0;
				nMotorEncoder[leftFront] = 0;
				mtr[0] = 0;
			}
			if((mtr[1] != 0) && (nMotorEncoder[leftRear] > mtr[1]))	{
				motor[leftRear] = 0;
				nMotorEncoder[leftRear] = 0;
				mtr[1] = 0;
			}
			if((mtr[2] != 0) && (nMotorEncoder[rightFront] > mtr[2]))	{
				motor[rightFront] = 0;
				nMotorEncoder[rightFront] = 0;
				mtr[2] = 0;
			}
			if((mtr[3] != 0) && (nMotorEncoder[rightRear] > mtr[3]))	{
				motor[rightRear] = 0;
				nMotorEncoder[rightRear] = 0;
				mtr[3] = 0;
			}
		}

		motor[leftFront	] = 0;
		motor[leftRear	] = 0;
		motor[rightFront] = 0;
		motor[rightRear	] = 0;

		wait1Msec(100);
	}
}
