#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S4,     LiftTouchSensor, sensorTouch)
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

//NXT brick button mapping
#define RIGHT 1
#define LEFT 2

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

int motorOverload(int joyValue, ){

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

task tensionLift(){
	motor[ScissorLeft] = 56;
	motor[ScissorRight] = 60;
	wait1Msec(1800);
	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;
}

void tensionControl(){
	StartTask(tensionLift, 7);
}

void armControl(){

	if(joy2Btn(RIGHT_BTN)){
		servo[ArmContRot] = 255;

 	}else if(joy2Btn(LEFT_BTN)){
 		servo[ArmContRot] = 0;

 	}else{
 		servo[ArmContRot] = 126;
 	}
}

void directDriveControl(int range){

	//motor speed and direction is directly translated from joystick position
	motor[LeftFront] = calcMotorPow(joystick.joy1_y1, range); //scale -range, +range
	motor[LeftRear]  = calcMotorPow(joystick.joy1_y1, range);
	motor[RightFront] = calcMotorPow(joystick.joy1_y2, range);
	motor[RightRear] = calcMotorPow(joystick.joy1_y2, range);
}

void hookDriveControl(int range){

	//motor speed is adjusted based on joystick direction
	//motor direction is based on the assigned motor value that is farthest from 0
	if(abs(joystick.joy1_y1) < abs(joystick.joy1_y2)){
		int joyValue1 = (joystick.joy1_y1 / joystick.joy1_y2) * (joystick.joy1_y2 / abs(joystick.joy1_y2));

		motor[LeftFront] = calcMotorPow(joyValue1, range);
		motor[LeftRear]  = calcMotorPow(joyValue1, range);
		motor[RightFront] = calcMotorPow(joystick.joy1_y2, range);
		motor[RightRear] = calcMotorPow(joystick.joy1_y2, range);

	}else if(abs(joystick.joy1_y2) > abs(joystick.joy1_y1)){
		int joyValue2 = (joystick.joy1_y2 / joystick.joy1_y1) * (joystick.joy1_y1 / abs(joystick.joy1_y1));

		motor[LeftFront] = calcMotorPow(joystick.joy1_y1, range);
		motor[LeftRear]  = calcMotorPow(joystick.joy1_y1, range);
		motor[RightFront] = calcMotorPow(joyValue2, range);
		motor[RightRear] = calcMotorPow(joyValue2, range);

	}else{
		if((joystick.joy1_y1 / joystick.joy1_y2) != 1){
			if(joystick.joy1_y1 > 0){
				motor[LeftFront] = calcMotorPow(joystick.joy1_y1, range);
				motor[LeftRear]  = calcMotorPow(joystick.joy1_y1, range);
				motor[RightFront] = -calcMotorPow(joystick.joy1_y2, range);
				motor[RightRear] = -calcMotorPow(joystick.joy1_y2, range);

			}else{
				motor[LeftFront] = -calcMotorPow(joystick.joy1_y1, range);
				motor[LeftRear] = -calcMotorPow(joystick.joy1_y1, range);
				motor[RightFront] = calcMotorPow(joystick.joy1_y2, range);
				motor[RightRear] = calcMotorPow(joystick.joy1_y2, range);
			}

		}else{
			motor[LeftFront] = calcMotorPow(joystick.joy1_y1, range);
			motor[LeftRear] = calcMotorPow(joystick.joy1_y1, range);
			motor[RightFront] = calcMotorPow(joystick.joy1_y2, range);
			motor[RightRear] = calcMotorPow(joystick.joy1_y2, range);
		}
	}
}

void liftControl(int range, int joyValue1, int joyValue2){

	if(joy2Btn(DOWN_BTN)){
		tensionControl();
	}

	if((joyValue1 > 30) || (joyValue2 > 30)){
		motor[ScissorLeft] = 92; //100
		motor[ScissorRight] = 100;

	}else if((joyValue1 < -30) || (joyValue2 < -30)){
		if(SensorValue(LiftTouchSensor) == 0){
			motor[ScissorLeft] = -92; //-100;
			motor[ScissorRight] = -100;

		}else{
			motor[ScissorLeft] = 0;
			motor[ScissorRight] = 0;
		}

	}else{
		motor[ScissorLeft] = 0;
		motor[ScissorRight] = 0;
	}
}

task main(){

	initializeRobot();
	waitForStart(); //comment whole line when testing and not running FCS

	while(true){

		getJoystickSettings(joystick); //get current joystick state with each update

	  //--- DRIVE MOTORS (Controller #1)

	  if(joy1Btn(L1) || joy1Btn(R1)){       //when a modifier button is pressed
	  	directDriveControl(50);

	  }else if(joy1Btn(L2) || joy1Btn(R2)){ //when a modifier button is pressed
	  	directDriveControl(100);

	  }else{                                //default driving mode
	  	directDriveControl(75);
	  }

		//--- PULLEY LIFT (Controller #2)

		if(joy2Btn(L1) || joy2Btn(R1)){ //slow
	  	liftControl(5, joystick.joy2_y1, joystick.joy2_y2);

	  }else if(joy2Btn(L2) || joy2Btn(R2)){ //fast
	  	liftControl(100, joystick.joy2_y1, joystick.joy2_y2);

	  }else{
	  	liftControl(50, joystick.joy2_y1, joystick.joy2_y2); //default
	  }

	  if(SensorValue(LiftTouchSensor) == 1){
	  	tensionControl();
	  }

	  //--- ARM CONTROL (Controller #2)

	  armControl();

	}
}
