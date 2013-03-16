#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     IRSeeker,       sensorI2CCustom)
#pragma config(Sensor, S3,     HTMUX,           sensorI2CCustom)
#pragma config(Sensor, S4,     LiftTouchSensor, sensorTouch)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorRight,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorLeft,   tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     RightRear,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     RightFront,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     LeftRear,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     LeftFront,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    BasketLeft,           tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    BasketRight,          tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring it up!"

	Written by Joe Quigley and Ethan Eymontt
	~The official 2013 Autonomous round program~
	Reads a file from the NXT and executes it.

	Revisions:
n		Rev 	Date		Notes
1		1.0		3/15/13	Initial version
*/

#include "stdbot.h"

#define B_BACK 0
#define B_RIGHT 1
#define B_LEFT 2
#define B_ORANGE 3

tMotor mtrs[] = {LeftFront, LeftRear, RightFront, RightRear, ScissorRight, ScissorLeft};
TServoIndex servos[] = {BasketLeft, BasketRight, ArmContRot};

int* getNextStep()	{
	//	note that initReadMode() must have been called before this function is called
	int stp[8];
	for(int i = 0; i < 8; i++)	{
		stp[i] = nextInt();
	}
	return stp;
}

int getSteps()	{
	bool cont = true;
	int nSteps = 0;

	nxtDisplayCenteredBigTextLine(0, "#STEPS:");
	while(cont)	{
		if(nNxtButtonPressed == B_LEFT)	{
			nSteps--;
		}
		else if(nNxtButtonPressed == B_RIGHT)	{
			nSteps++;
		}
		else if(nNxtButtonPressed == B_ORANGE)	{
			cont = false;
		}

		nxtDisplayCenteredTextLine(3, "%i", nSteps);
		wait1Msec(75);
	}

	return nSteps;
}

task main()
{
	initReadMode("auton.txt", 1024);

	int nSteps = getSteps();

	for(int i = 0; i < nSteps; i++)	{
		int* stps = getNextStep();
		for(int m = 0; m < 8; m++)	{
			writeDebugStreamLine("stps[%i] : %i", i, stps[i]);
		}
	}
}
