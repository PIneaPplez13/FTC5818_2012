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
	Runs forward parallel to the tree until the IRSeeker finds the beacon in sector 8.
	Then, turns to fact tree, moves up, moved EOPD to correct alignment, and finally
	dispenses the ring.

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
10	2.4		3/15/23 Forked new version called AutonAlt, current now has accurate turning, failsafe, and EOPD support
11	2.4		3/16/23 Scratch EOPD support... (469... 469... 469... 460..+9...)
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

#define EOPD msensor_S3_4

//	papaya banana watermelon rhubarb
#define loopi(x, y) (int i = x; i < y; i++)

tMotor mtrs[] = {LeftFront, LeftRear, RightFront, RightRear, ScissorLeft, ScissorRight};
TServoIndex servos[] = {BasketLeft, BasketRight, ArmContRot};

/*
+=======================================================+
+BASIC TURNING GROUPS																		+
+=======================================================+
Start Pos			LeftFront	LeftRear	RightFr		RightRear*/
int Left[] = {100, 			100, 			-100, 		-100};
int Fwd[]	 = {100,			100,			100,			100};
int Right[]= {-100,			-100,			100,			100};

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

	nxtDisplayTextLine(4, "Start pos is:");
	nxtDisplayCenteredTextLine(5, positions[pos]);

	wait1Msec(500);

	return pos;
}

void printDiag()	{
	//	Prints a replacement for the debug dialog on specific lines
	nxtDisplayTextLine(0, "FTC Team 5818");
	nxtDisplayTextLine(1, "Battery Voltage (v):");
	nxtDisplayTextLine(2, "NXT %fv", (nAvgBatteryLevel/1000.0));
	nxtDisplayTextLine(3, "BOT %fv", (externalBatteryAvg/1000.0));
}

task diag()	{
	//	Continually prints dialog
	while(true)	{
		printDiag();
		wait1Msec(50);
	}
}

task main()	{
	delay = 0;
	disableDiagnosticsDisplay();
	startPos = getStartPos();
	StartTask(diag, 9);
	initRobot();
	waitForStart();									//-----------------------------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	wait1Msec(delay);
	servo[BasketLeft] = 126;
	servo[BasketRight] = 126;
	for loopi(0, 4)	{
		motor[mtrs[i]] = 80;
	}
	PlayTone(523, 50);
	readIRSeeker(IRSeeker, ir);
	if(startPos == 0)	{
		while(ir.dir != 8)	{
			readIRSeeker(IRSeeker, ir);
		}
	}
	else	{
		while(ir.dir != 8)	{
			readIRSeeker(IRSeeker, ir);
		}
	}
	wait1Msec(150);
	PlayTone(523, 50);
	for loopi(0, 4)	{
		motor[mtrs[i]] = 0;
	}
	PlayTone(523, 50);
	wait1Msec(500);

	nMotorEncoder[LeftFront] = 0;

	/*if(startPos == LEFT)	{
		for loopi(0, 4)	{
			motor[mtrs[i]] = Right[i]; //was Left[i]
		}
	}
	else	{*/
		for loopi(0, 4)	{
			motor[mtrs[i]] = Left[i]; //was Right[i]1
		}
	//}

	while(abs(nMotorEncoder[LeftFront]) < 4000)	{};

	for loopi(0, 4)	{
		motor[mtrs[i]] = 0;
	}

	nMotorEncoder[ScissorRight] = 0;

	motor[ScissorLeft] = 100;
	motor[ScissorRight] = 100;

	beginNewTimer(1000);

	while((getElapsed() > 0)){};

	endActiveTimer();

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	wait1Msec(250);

	motor[ScissorLeft] = -100;
	motor[ScissorRight] = -100;

	beginNewTimer(250);

	while((getElapsed() > 0)){};

	endActiveTimer();

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	wait1Msec(500);

	for loopi(0, 4)	{
		motor[mtrs[i]] = 80;
	}

	nMotorEncoder[LeftFront] = 0;

	beginNewTimer(1000);

	while((getElapsed() > 0) && (abs(nMotorEncoder[LeftFront]) < 1000)){};

	endActiveTimer();

	for loopi(0, 4)	{
		motor[mtrs[i]] = 0;
	}

	servo[ArmContRot] = 0;
	wait1Msec(1500);
	servo[ArmContRot] = 126;

	motor[ScissorLeft] = -100;
	motor[ScissorRight] = -100;

	beginNewTimer(250);

	while((getElapsed() > 0)){};

	endActiveTimer();

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	PlayTone(523, 125);
}
