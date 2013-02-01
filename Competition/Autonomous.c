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

#define nSTEPS 3
#define LEFT 2
#define RIGHT 1
#define ORANGE 3

#include "stdbot.h"

int steps[nSTEPS][4] = {{950, 950, 950, 950}, {1350, 1350, -1350, -1350}, {3900, 3900, 3900, 3900}};
//int LEFT_FIRST[nSTEPS][4] = {{950, 950, 950, 950}, {1350, 1350, -1350, -1350}, {3900, 3900, 3900, 3900}};
//int *steps[nSTEPS][4];

tMotor mtrs[4] = {LeftRear, LeftFront, RightRear, RightFront};
tMotor *mtr = mtrs;

int nStep = 0;
int i = 0;
bool cont = true;
bool mtrRunning[4] = {true, true, true, true};

bool pressed = false;

/*void selectMode()	{
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
}*/

task main()
{
	//selectMode();

	nMotorEncoder[LeftFront	] = 0;
	nMotorEncoder[LeftRear	] = 0;
	nMotorEncoder[RightFront] = 0;
	nMotorEncoder[RightRear	] = 0;
	nMotorEncoder[ScissorRight] = 0;

	while(nStep < nSTEPS)	{
		beginNewTimer(5000);
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
			}
		}
		for(i = 0; i < 4; i++)	{
			mtrRunning[i] = true;
		}
		cont = true;
		nStep++;
		i = 0;
	}
	motor[ScissorLeft] = 100;
	motor[ScissorRight] = 100;

	while((nMotorEncoder[ScissorRight] < 2250)){};

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;

	wait1Msec(1000);

	servo[ArmContRot] = 255;
	wait1Msec(1500);
	servo[ArmContRot] = 126;

	motor[ScissorLeft] = -100;
	motor[ScissorRight] = -100;

	while((nMotorEncoder[ScissorRight] > 500)){};

	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;
}
