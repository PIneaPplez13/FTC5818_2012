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
#pragma config(Motor,  mtr_S1_C3_2,     LeftFront,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    BasketLeft,           tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    BasketRight,          tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring It Up!"

	Written by Ethan Eymontt and Joe Quigley
	~ The official 2013 Teleoperated round program ~

	The teleoperated code begins by defining constants that affect gamepad mapping and motor sync
	speeds. Each section before the main task is organized into sections that mainly cover the drive,
	arm, and lift aspects of the robot. In the running task, the functions that handle those categories
	are called. The gamepad input source is usually passed to each handle (i.e. control) function
	to keep the method of having seperate "drive system" and "fine movement" controlling in practice.

	~ Updated Revision History ~
  n:   Rev:   Date: 	    Notes:
	3 	 -      12/26/12    First stable version released
	4    v4.0	  12/31/12	  Used in qualifying competition (Jan 06 '13)
	6	   v5 	  01/15/13	  -
	4	   v5.1	  01/17/13	  Much error handling
	7	   v6 	  01/25/13	  Greater control over arm is achieved
	8	   v7 	  02/01/13	  Universal constants integrated into program; drive system troubleshooted
	11   v7.3   02/02/13    Used in second qualifying competition (Feb 02 '13)
	15	 v9 	  02/15/13	  Multiplexer is reconfigured; tension control functions are implemented
	17 	 v9.9	  02/21/13	  -
	18	 v10	  02/22/13	  Simplified code greatly; new gamepad mapping
	19	 v10b		03/15/13		New fork: Autonomous recorded
*/

#include "JoystickDriver.c"
#include "stdbot.h"

#define    UP_BTN 4
#define  DOWN_BTN 2
#define  LEFT_BTN 1
#define RIGHT_BTN 3

#define L1 6
#define L2 8
#define R1 5
#define R2 7

#define    HAT_UP 0
#define  HAT_DOWN 4
#define  HAT_LEFT 6
#define HAT_RIGHT 2

#define  LEFT_LIFT_CON 100
#define RIGHT_LIFT_CON 94

#define   DEADBAND 10
#define DEADBAND_B 45
#define MAX_JOY_VALUE 127.0

//--- TENSION CONTROL

task tensionLift(){

	motor[ScissorLeft] = 60 * LEFT_LIFT_CON;
	motor[ScissorRight] = 60 * RIGHT_LIFT_CON;
	wait1Msec(1800);
	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;
}

void tensionControl(){
	StartTask(tensionLift, 7);
}

//--- INITIALIZATION

int output = 126;

void initializeRobot(){

	nMotorEncoder[ScissorLeft] = 0;
  nMotorEncoder[ScissorRight] = 0;

  StopTask(tensionLift);

  servo[ArmContRot] = 126;
  servo[BasketLeft] = 255;
  servo[BasketRight] = 255;
}

//--- GENERAL MOTOR CALCULATIONS

int calcMotorPow(int joyValue, int maxMotorSpeed){

	if(abs(joyValue) < DEADBAND){
		return 0;
	}

	float ratio = pow(joyValue, 2) / pow(MAX_JOY_VALUE, 2);
	int motorSpeed = (ratio * maxMotorSpeed) * (joyValue / abs(joyValue));
	return motorSpeed;
}

//--- DRIVE MOTORS

void moveLeftMotors(int motorSpeed){
	motor[LeftFront] = motorSpeed;
	motor[LeftRear] = motorSpeed;
}

void moveRightMotors(int motorSpeed){
	motor[RightFront] = motorSpeed;
	motor[RightRear] = motorSpeed;
}

void directDriveControl(int range, int joyValueY1, int joyValueY2){
	moveLeftMotors(calcMotorPow(joyValueY1, range));
	moveRightMotors(calcMotorPow(joyValueY2, range));
}

//--- LIFT CONTROL

void liftControl(int singleSpeed, int doubleSpeed, int joyValueY){
	int dir = (joyValueY / abs(joyValueY));

	if(abs(joyValueY) > 30){
		if(joy2Btn(L2)){
			motor[ScissorLeft] = singleSpeed * dir * LEFT_LIFT_CON;

		}else if(joy2Btn(R2)){
			motor[ScissorRight] = singleSpeed * dir * RIGHT_LIFT_CON;

		}else{
			motor[ScissorLeft] = doubleSpeed * dir * LEFT_LIFT_CON;
			motor[ScissorRight] = doubleSpeed * dir * RIGHT_LIFT_CON;
		}

	}else{
		motor[ScissorLeft] = 0;
		motor[ScissorRight] = 0;
	}
}

void liftHatControl(int singleSpeed, int doubleSpeed){

	int dir = 1;

	if(joystick.joy2_TopHat == HAT_UP){
		dir = 1;
	}else{
		dir = -1;
	}

	if(joy2Btn(L2)){
		motor[ScissorLeft] = singleSpeed * dir * LEFT_LIFT_CON;
	}else if(joy2Btn(R2)){
		motor[ScissorRight] = singleSpeed * dir * RIGHT_LIFT_CON;
	}else{
		motor[ScissorLeft] = doubleSpeed * dir * LEFT_LIFT_CON;
		motor[ScissorRight] = doubleSpeed * dir * RIGHT_LIFT_CON;
	}
}

//--- ARM SERVO(S)

void delayServo(int delay_constant, int deltaOutput){

	int a = ServoValue[BasketLeft];
	int b = ServoValue[BasketRight];

	for(int dave = a; dave <= a + 1; dave++){
		a = ServoValue[BasketLeft];
		b = ServoValue[BasketRight];
		servo[BasketLeft] = a + deltaOutput;
		servo[BasketRight] = b + deltaOutput;
		wait1Msec(delay_constant / 1000);
	}
}

void armControl(){

	if(joy2Btn(RIGHT_BTN)){
		servo[ArmContRot] = 0;

	}else if(joy2Btn(DOWN_BTN)){
		servo[ArmContRot] = 255;

	}else{
		servo[ArmContRot] = 126;
	}
}

void armArtControl(int &output){

	if(joystick.joy2_x2 > DEADBAND_B || joystick.joy2_TopHat == HAT_RIGHT){
		delayServo(1, 3);
		if(output > 255){
			output = 255;
		}

	}else if(joystick.joy2_x2 < -DEADBAND_B || joystick.joy2_TopHat == HAT_LEFT){
		delayServo(1, -3);
		if(output < 0){
			output = 0;
		}

	}else{
		output = output;
	}
}

//--- MAIN PROGRAM

task main(){

	initWriteMode("auton.txt", 1024, true);
	initializeRobot();

	waitForStart();

	while(true){

		getJoystickSettings(joystick);

		//--- DRIVE MOTORS (Controller #1)

		if(joy1Btn(L1) || joy1Btn(R1)){
			directDriveControl(20, joystick.joy1_y1, joystick.joy1_y2);

		}else if(joy1Btn(L2) || joy1Btn(R2)){
			directDriveControl(100, joystick.joy1_y1, joystick.joy1_y2);

		}else{
			directDriveControl(70, joystick.joy1_y1, joystick.joy1_y2);
		}

		//--- PULLEY LIFT (Controller #2)

		liftControl(50, 100, joystick.joy2_y1); //singleSpeed, doubleSpeed, joystick

		//--- TENSION CONTROL

		if((SensorValue(LiftTouchSensor) == 1) || (joy2Btn(UP_BTN))){
			tensionControl();
		}

		//--- ARM CONTROL (Controller #2)

		armControl();
		armArtControl(output);

		if(joy2Btn(L1) || joy2Btn(R1)){
			servo[BasketLeft] = 126;
			servo[BasketRight] = 126;
		}

		//--- WRITE CONTROL (Controller #2)

		if(joystick.joy2_TopHat == HAT_UP)	{
			writeInt(nMotorEncoder[LeftFront]);
			writeInt(nMotorEncoder[LeftRear]);
			writeInt(nMotorEncoder[RightFront]);
			writeInt(nMotorEncoder[RightRear]);
			writeInt(nMotorEncoder[ScissorRight]);
			writeInt(nMotorEncoder[ScissorLeft]);
			PlayTone(523, 15);
			wait1Msec(100);
		}

		if(joystick.joy2_TopHat == HAT_DOWN)	{
			nMotorEncoder[LeftFront] = 0;
			nMotorEncoder[LeftRear] = 0;
			nMotorEncoder[RightFront] = 0;
			nMotorEncoder[RightRear] = 0;
			nMotorEncoder[ScissorRight] = 0;
			nMotorEncoder[ScissorLeft] = 0;
		  PlayTone(523, 15);
			wait1Msec(100);
		}
	}
}
