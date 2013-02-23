#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
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

*/

#include "stdbot.h"
#include "JoystickDriver.c"

tMotor mtrs[] = {LeftFront, LeftRear, RightFront, RightRear, ScissorLeft, ScissorRight};
TServoIndex servos[] = {BasketLeft, BasketRight, ArmContRot};
/*
+=======================================================+
+ENCODER GROUPS TO GET THE ROBOT TO THE TREE						+
+=======================================================+
Start Pos					LeftFront	LeftRear	RightFr		RightRear*/
int SLeft[][]	= {{500,			500,			0,				0}};
int SCorner[][]= {{500,			500,			500,			500}};
int SRight[][] =	{{0,				0,				500,			500}};
/*
+=======================================================+
+ENCODER GROUPS TO GET THE ROBOT TO A COLUMN						+
+=======================================================+
Start Pos					LeftFront	LeftRear	RightFr		RightRear*/
int	CLeft[][]	= {{500,			500,			0,				0}};
int CCtr[][]	= {{500,			500,			500,			500}};
int CRight[][]= {{0,				0,				500,			500}};

int* encoders;

tIRSeek ir;

enum {BACK, RIGHT, LEFT, ORANGE};
/*enum {
	LEFT, CORNER, RIGHT
}*/

int startPos;

void initRobot()	{
	Servo[servos[0]] = 255;
	Servo[servos[1]] = 255;
	Servo[servos[2]] = 126;

	for(int i = 0; i < (sizeof(mtrs)/sizeof(tMotor)); i++)	{
		Motor[mtrs[i]] = 0;
	}
}

int findColumn()	{
	readIRSeeker(IRSeeker, ir);
	return(ir.dir - 4);
}

int getStartPos()	{
	bool pressed = false;
	int pos = -1;

	nxtDisplayTextLine(0, "5818 Autonomous");
	nxtDisplayTextLine(1, "ENTER START POS");
	nxtDisplayCenteredTextLine(2, "< ^ >");
	beginNewTimer(5000);
	while(!pressed || (getElapsed() > 0))	{
		if(nNxtButtonPressed(buttons.LEFT))	{
			pressed = true;
			pos = positions.LEFT;
		}
		else if(nNxtButtonPressed(buttons.ORANGE))	{
			pressed = true;
			pos = positions.CORNER;
		}
		else if(nNxtButtonPressed(buttons.RIGHT))	{
			pressed = true;
			pos = positions.RIGHT;
		}
	}
	nxtDisplayTextLine(3, "Start pos is:");
	nxtDisplayCenteredTextLine(4, positions.pos);
}

task main()	{
	startPos = getStartPos();
	initRobot();
	waitForStart();
}
