#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorRight,  tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     LeftRear,      tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     LeftFront,     tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     RightRear,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     RightFront,    tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

#include "JoystickDriver.c" //to handle Bluetooth messages

//logitech controller mapping
#define    UP_BTN 4
#define  DOWN_BTN 2
#define  LEFT_BTN 1
#define RIGHT_BTN 3

#define L1 6
#define L2 8
#define R1 5
#define R2 7

//we don't want the deadband to be too much
#define DEADBAND 10
#define MAX_JOY_VALUE 127.0

void initializeRobot(){

	//reset motor encoders on scissor lift motors
	nMotorEncoder[ScissorLeft] = 0;
  nMotorEncoder[ScissorRight] = 0;

  //initialize servo to starting position
  servo[ArmContRot] = 126;
}

int calcMotorPow(int joyValue, int maxMotorSpeed){

	//dead-zone included to preserve motors
	if(abs(joyValue) < DEADBAND){
		return 0;
	}

	//calculate motor speed
	float ratio = pow(joyValue, 2) / pow(MAX_JOY_VALUE, 2);
	int motorSpeed = (ratio * maxMotorSpeed) * (joyValue / abs(joyValue));

	return motorSpeed; //motor speed given to motor
}

void armControl(){

	if(joy2Btn(RIGHT_BTN)){
		servo[ArmContRot] = 255;
 	}else if(joy2Btn(DOWN_BTN)){
 		servo[ArmContRot] = 0;
 	}else{
 		servo[ArmContRot] = 126;
 	}
}

void driveControl(int range){

	//motor speed and direction is directly translated from joystick position
	motor[LeftFront] = calcMotorPow(joystick.joy1_y1, range); //scale -range, +range
	motor[LeftRear]  = calcMotorPow(joystick.joy1_y1, range);
	motor[RightFront] = calcMotorPow(joystick.joy1_y2, range);
	motor[RightRear]  = calcMotorPow(joystick.joy1_y2, range);
}

void liftControl(int range){

	//motor direction is translated from joystick position
	//general motor speed it determined from modifier buttons
	motor[ScissorLeft] = calcMotorPow(joystick.joy2_y1, range); //scale -range to +range
	motor[ScissorRight] = calcMotorPow(joystick.joy2_y2, range);
}

task main(){

	initializeRobot();
	waitForStart(); //comment whole line when testing and not running FCS

	while(true){

		getJoystickSettings(joystick); //get current joystick state with each update

		//--- SCISSOR LIFT (Controller #2)

		if(joy2Btn(L1) || joy2Btn(R1)){ //slow
	  	liftControl(20);
	  }else if(joy2Btn(L2) || joy2Btn(R2)){ //fast
	  	liftControl(100);
	  }else{
	  	liftControl(80); //default
	  }

	  //--- DRIVE MOTORS (Controller #1)

	  if(joy1Btn(L1) || joy1Btn(R1)){
	  	driveControl(50);
	  }else if(joy1Btn(L2) || joy1Btn(R2)){
	  	driveControl(100);
	  }else{
	  	driveControl(75);
	  }

	  //--- ARM CONTROL (Controller #2)

	  armControl();

	}
}
