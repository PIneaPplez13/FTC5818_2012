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

#include "JoystickDriver.c"

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

int output = 126;

//number of debug options
#define OPTIONS 3

//NXT button codes
#define LEFT 2
#define RIGHT 1
#define ORANGE 3
#define NO_BTN -1

string components[OPTIONS];
int size = OPTIONS;

//--- DEBUG MENU

int debugMenu(){

	int index = 1;
	bool selected = false;

	while(!selected){

		wait1Msec(10);
		eraseDisplay();
		nxtDisplayCenteredTextLine(1, "Debug Menu");
		nxtDisplayTextLine(3, "%d", index);
		nxtDisplayTextLine(4, "%s", components[index]);
		nxtDisplayCenteredTextLine(6, "<| cycle |>");

  	if(nNxtButtonPressed == LEFT){
  		if(index > 0){
  			index--;
  		}else {index = OPTIONS;}

		}if(nNxtButtonPressed == RIGHT){
  		if(index < OPTIONS){
  			index++;
  		}else {index = 0;}

  	}if(nNxtButtonPressed == ORANGE){
  		selected = true;
  	}
	}
	return index;
}

//--- DRIVE TEST

task driveDebug(){
	disableDiagnosticsDisplay();
	while(nNxtButtonPressed == NO_BTN){

	}
}

void driveTest(){
	StartTask(driveDebug, 7);
}

//--- ARM TEST

task armDebug(){
	servo[ArmContRot] = 126;
  servo[BasketLeft] = 255;
  servo[BasketRight] = 255;
  disableDiagnosticsDisplay();
	while(nNxtButtonPressed == NO_BTN){

	}
}

void armTest(){
	StartTask(armDebug, 7);
}

//--- LIFT TEST

task liftDebug(){
	nMotorEncoder[ScissorLeft] = 0;
  nMotorEncoder[ScissorRight] = 0;
  disableDiagnosticsDisplay();
	while(nNxtButtonPressed == NO_BTN){

	}
}

void liftTest(){
	StartTask(liftDebug, 7);
}

//--- MAIN PROGRAM

task main(){

	size = OPTIONS;
	string test;

	components[0] = "Drive_Sys";
	components[1] = "Arm_Servos";
	components[2] = "Lift_Sys";

	test = debugMenu();

	if(test == "0"){
		driveTest();
	}
	if(test == "1"){
		armTest();
	}
	if(test == "2"){
		liftTest();
	}
}
