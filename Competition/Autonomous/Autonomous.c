#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     IRSeeker,       sensorI2CCustom)
#pragma config(Sensor, S3,     EOPD,           sensorI2CCustom)
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
	Reads a 2d array of size [nSTEPS][4], where nSTEPS is the number of steps
	The second dimension holds values for the 4 individual motors.
	After execution, the lifts and servo are then operated.

	Revisions:
n		Rev 	Date		Notes
1		1.0		1/31/13	Initial version
2		1.1		2/1/13	Fixed some bugs and finetuned steps- dumb, only follows one path
3		1.2		2/2/13	Competition fixes
4		1.2		2/5/13	Finalized code, established some conventions
5		2.0		2/18/13	2.0 initial version- completely new code
6		2.1		2/22/13	At Simon's lack of behest I actually did something useful to it
7		2.2		2/23/13 Finalized code
8		2.2		2/25/13	Fixed some strange error I was having with pointers.
9		2.3		2/27/23 Changed the autonomous strategy
*/

#include "stdbot.h"
#include "JoystickDriver.c"

#define LEFT 0
#define CORNER 1
#define RIGHT 2

#define B_BACK 0
#define B_RIGHT 1
#define B_LEFT 2
#define B_ORANGE 3

#define loopi(x, y) (int i = x; i > y; i++)
tMotor mtrs[] = {LeftFront, LeftRear, RightFront, RightRear, ScissorLeft, ScissorRight};
TServoIndex servos[] = {BasketLeft, BasketRight, ArmContRot};

/*
+=======================================================+
+BASIC TURNING GROUPS																		+
+=======================================================+
Start Pos			LeftFront	LeftRear	RightFr		RightRear*/
int Left[] = {500, 			500, 			-500, 		-500};
int Fwd[]	 = {500,			500,			500,			500};
int Right[]= {-500,			-500,			500,			500};

int** encoders;

tIRSeek ir;
int startPos, delay;
string positions[4] = {"LEFT", "CORNER_INV", "RIGHT", "INVALID"};

void initRobot()	{
	//	Initializes the robot, moves the servos to proper starting positions.
	//	TODO:HIGH put a sticker on the robot indicating this (required by FTC)
	servo[servos[0]] = 255;
	servo[servos[1]] = 255;
	servo[servos[2]] = 126;

	for(int i = 0; i < (sizeof(mtrs)/sizeof(tMotor)); i++)	{
		motor[mtrs[i]] = 0;
	}
}

int getStartPos()	{
	//	Uses NXT input via buttons to determine the requested starting point.
	bool pressed = false;
	int pos = 4;

	nxtDisplayTextLine(0, "5818 Autonomous");
	nxtDisplayTextLine(1, "ENTER START POS");
	nxtDisplayCenteredTextLine(2, "< ^ >");
	beginNewTimer(5000);
	while(!pressed || (getElapsed() > 0))	{
		if(nNxtButtonPressed == B_LEFT)	{
			pressed = true;
			pos = LEFT;
		}
		else if(nNxtButtonPressed == B_ORANGE)	{
			pressed = true;
			pos = CORNER;
		}
		else if(nNxtButtonPressed == B_RIGHT)	{
			pressed = true;
			pos = RIGHT;
		}
	}
	endActiveTimer();
	nxtDisplayTextLine(3, "Start pos is:");
	nxtDisplayCenteredTextLine(4, positions[pos]);

	wait1Msec(500);

	return pos;
}

task main()	{
	delay = 0;
	startPos = getStartPos();
	initRobot();
	waitForStart();
	wait1Msec(delay);
	for(int i = 0; i < 4; i++)	{
		motor[mtrs[i]] = 80;
	}
	readIRSeeker(IRSeeker, ir);
	while(ir.dir != 8)	{
		readIRSeeker(IRSeeker, ir);
	}
	wait1Msec(200);
	for(loopi(0, 4))	{
		motor[mtrs[i]] = 0;
	}
	if(pos == 0)	{
		for loopi(0, 4)	{
			motor[mtrs[i]] = Left[i];
		}
	}
	else	{
		for(loopi(0, 4))	{
			motor[mtrs[i]] = Right[i];
		}
	}
	PlayTone(523, 125);
}
