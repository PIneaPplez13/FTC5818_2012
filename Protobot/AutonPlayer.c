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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "stdbot_d.h"

#define nSTEPS 3

bool cont = true;
int mtr[5] = {0,0,0,0,0};
//int *mref = mtr;

task main()
{
	/*for(; mref < mtr+5; mref++)	{
		*mref = 0;
	}*/

	initReadMode("auto.txt", 1024);

	nMotorEncoder[LeftFront	] = 0;
	nMotorEncoder[LeftRear	] = 0;
	nMotorEncoder[RightFront] = 0;
	nMotorEncoder[RightRear	] = 0;
	nMotorEncoder[ScissorLeft] = 0;
	nMotorEncoder[ScissorRight] = 0;

	for(int i = 0; i < nSTEPS; i++)	{
		PlayTone(523, 150);
		writeDebugStreamLine("Step: %i/%i", nextInt()+1, nSTEPS);
		mtr[0] = nextInt() * 1000;
		mtr[1] = nextInt() * 1000;
		mtr[2] = nextInt() * 1000;
		mtr[3] = nextInt() * 1000;
		mtr[4] = nextInt() * 1000;

		writeDebugStreamLine("Left Front:  %i", mtr[0]);
		writeDebugStreamLine("Left Rear:   %i", mtr[1]);
		writeDebugStreamLine("Right Front: %i", mtr[2]);
		writeDebugStreamLine("Right Front: %i", mtr[3]);
		writeDebugStreamLine("Scissors: %i", mtr[4]);

		motor[LeftFront	] = 100 * sgn(mtr[0]);
		motor[LeftRear	] = 100 * sgn(mtr[1]);
		motor[RightFront] = 100 * sgn(mtr[2]);
		motor[RightRear	] = 100 * sgn(mtr[3]);
		motor[ScissorLeft] = 100 * sgn(mtr[4]);
		motor[ScissorRight] = 100 * sgn(mtr[4]);

		while(cont)	{
			if((mtr[0] != 0) && (abs(nMotorEncoder[LeftFront]) > abs(mtr[0])))	{
				motor[LeftFront] = 0;
				nMotorEncoder[LeftFront] = 0;
				mtr[0] = 0;
			}
			if((mtr[1] != 0) && (abs(nMotorEncoder[LeftRear]) > abs(mtr[1])))	{
				motor[LeftRear] = 0;
				nMotorEncoder[LeftRear] = 0;
				mtr[1] = 0;
			}
			if((mtr[2] != 0) && (abs(nMotorEncoder[RightFront]) > abs(mtr[2])))	{
				motor[RightFront] = 0;
				nMotorEncoder[RightFront] = 0;
				mtr[2] = 0;
			}
			if((mtr[3] != 0) && (abs(nMotorEncoder[RightRear]) > abs(mtr[3])))	{
				motor[RightRear] = 0;
				nMotorEncoder[RightRear] = 0;
				mtr[3] = 0;
			}
			if((mtr[4] != 0) && (abs(nMotorEncoder[ScissorRight]) > abs(mtr[4])))	{
				motor[ScissorLeft] = 0;
				motor[ScissorRight] = 0;
				nMotorEncoder[ScissorRight] = 0;
				mtr[4] = 0;
			}

			if((mtr[0] == 0) && (mtr[1] == 0) && (mtr[2] == 0) && (mtr[3] == 0) && (mtr[4] == 0))	{
				cont = false;
			}
		}

		cont = true;

		motor[LeftFront	] = 0;
		motor[LeftRear	] = 0;
		motor[RightFront] = 0;
		motor[RightRear	] = 0;
		motor[ScissorLeft] = 0;
		motor[ScissorRight] = 0;

		wait1Msec(100);
	}
}
