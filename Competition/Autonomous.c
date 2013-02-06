#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, openLoop)
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

	Written by Joe Quigley and Ethan Eymontt
	~The official 2013 Autonomous round program~
	Reads a 2d array of size [nSTEPS][4], where nSTEPS is the number of steps
	The second dimension holds values for the 4 individual motors.
	After execution, the lifts and servo are then operated.

	Revisions:
n		Rev 	Date		Notes
1		0.0		1/31/13	Initial version
2		0.1		2/1/13	Fixed some bugs and finetuned steps- dumb, only follows one path
3		0.1		2/2/13	Competition fixes
4		0.1		2/5/13	Finalized code, established some conventions

*/

#define nSTEPS 3
#define LEFT 2
#define RIGHT 1
#define ORANGE 3

#include "stdbot.h"
#include "JoystickDriver.c"

/*	==========================
 *	Motor Action Groups (MAGs)
 *	==========================
 *	These detail motor actions to take during normal execution of the Autonomus round.
 *	They are formatted thusly:
 *	An array of 4-element arrays, with each element of the 4-element arrays containing
 *	encoder steps to take for each motor. The program will not move on until these
 *	values are met. They are in the following order: LeftRear, LeftFront, RightRear,
 *	RightFront.
*/

int steps[nSTEPS][4] = {{840, 840, 840, 840}, {1350, 1350, -1350, -1350}, {3900, 3900, 3900, 3900}};

/*	=========================
 *	Special Action Groups(SAGs)
 *	=========================
 *	These are functions that move particular elements of the robot. They are predefined
 *	and each sequential function is to be formatted thusly:
 *	int SAGn()
 *	where n is the number of the SAG, numbered first to last in order of execution. It
 *	returns a 1 if an error is likely, 0 if execution completed successfully, and -1 if
 *	execution was known to fail.
*/

int SAG1()	{
	//	Moves the lifts up
	motor[ScissorLeft] = 100;
	motor[ScissorRight] = 100;

	while((nMotorEncoder[ScissorRight] < 2250)){};

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	return 0;
}

int SAG2()	{
	//	Moves the servo
	servo[ArmContRot] = 255;
	wait1Msec(1500);
	servo[ArmContRot] = 126;

	return 0;
}

int SAG3()	{
	//	Moves the lifts down
	motor[ScissorLeft] = -100;
	motor[ScissorRight] = -100;

	while((nMotorEncoder[ScissorRight] > 500)){};

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	return 0;
}

//	Motor cycler

tMotor mtrs[4] = {LeftRear, LeftFront, RightRear, RightFront};
tMotor *mtr = mtrs;

//	Global variables

int nStep = 0;
int i = 0;
bool cont = true;
bool mtrRunning[4] = {true, true, true, true};

/*
void selectMode()	{
	nxtDisplayTextLine(1, "Select Mode");
	nxtDisplayTextLine(2, "LEFT- left side");
	nxtDisplayTextLine(3, "RIGHT- right side");

	while(pressed == false){

  	if(nNxtButtonPressed == LEFT){
  		steps = &LEFT_FIRST;
  		pressed = true;

  	}if(nNxtButtonPressed == RIGHT){
  		steps = &RIGHT_FIRST;
  		pressed = true;
  	}
  }

  eraseDisplay();
}
*/

task main()
{
	//selectMode();
	waitForStart();

	nMotorEncoder[LeftFront	] = 0;
	nMotorEncoder[LeftRear	] = 0;
	nMotorEncoder[RightFront] = 0;
	nMotorEncoder[RightRear	] = 0;
	nMotorEncoder[ScissorRight] = 0;

	wait1Msec(8000);
	beginNewTimer(5000);

	while(nStep < nSTEPS)	{
		motor[LeftRear	] = 50 * sgn(steps[nStep][0]);
		motor[LeftFront	] = 50 * sgn(steps[nStep][1]);
		motor[RightRear ] = 50 * sgn(steps[nStep][2]);
		motor[RightFront] = 50 * sgn(steps[nStep][3]);
		nMotorEncoder[LeftFront	] = 0;
		nMotorEncoder[LeftRear	] = 0;
		nMotorEncoder[RightFront] = 0;
		nMotorEncoder[RightRear	] = 0;
		i = 0;
		while(cont)	{
			for(i = 0; i < 4; i++)	{
				if(mtrRunning[i] && (abs(nMotorEncoder[*(mtr + i)]) > abs(steps[nStep][i])))	{
					motor[*(mtr + i)] = 0;
					mtrRunning[i] = false;
				}
			}
			if(((!mtrRunning[0]) && (!mtrRunning[1]) && (!mtrRunning[2]) && (!mtrRunning[3])) || (getElapsed() <= 0))	{
				cont = false;
				endActiveTimer();
				beginNewTimer(5000);
			}
		}
		for(i = 0; i < 4; i++)	{
			mtrRunning[i] = true;
		}
		cont = true;
		nStep++;
		i = 0;
	}

	SAG1();

	wait1Msec(1000);

	SAG2();
	SAG3();
}
