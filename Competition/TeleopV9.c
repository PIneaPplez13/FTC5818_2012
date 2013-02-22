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

//handle Bluetooth messages
#include "JoystickDriver.c"

//logitech gamepad mapping
#define    UP_BTN 4
#define  DOWN_BTN 2
#define  LEFT_BTN 1
#define RIGHT_BTN 3

#define L1 6
#define L2 8
#define R1 5
#define R2 7

//lift motor synch constants
#define  leftLiftCon 100
#define rightLiftCon 100

//NXT brick button mapping
#define RIGHT 1
#define  LEFT 2

//other useful shortcuts
#define DEADBAND 10
#define MAX_JOY_VALUE 127.0

//--- INITIALIZATION

void initializeRobot(){

	//reset motor encoders on scissor lift motors
	nMotorEncoder[ScissorLeft] = 0;
    nMotorEncoder[ScissorRight] = 0;

    //initialize servo with no speed
    servo[ArmContRot] = 126;
}

//--- GENERAL MOTOR CALCULATIONS

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

//--- DRIVE MOTORS

void moveLeftMotors(int motorSpeed){
    //apply power to left drive motors
    motor[LeftFront] = motorSpeed;
    motor[LeftRear] = motorSpeed;
}

void moveRightMotors(int motorSpeed){
    //apply power to right drive motors
    motor[RightFront] = motorSpeed;
    motor[RightRear] = motorSpeed;
}

void directDriveControl(int range, int joyValue1, int joyValue2){

	//motor speed and direction is directly translated from joystick position
	moveLeftMotors(calcMotorPow(joyValue1, range));
	moveRightMotors(calcMotorPow(joyValue2, range));
}

//--- LIFT MOTORS

task tensionLift(){
	//raise lift for a brief amount of time
	motor[ScissorLeft] = 60 * leftLiftCon;
	motor[ScissorRight] = 60 * rightLiftCon;
	wait1Msec(1800);
	motor[ScissorLeft] = 0;
	motor[ScissorRight] = 0;
}

void tensionControl(){
	StartTask(tensionLift, 7);
}

void rightControl(int range, int joyValue1, int joyValue2){

	if((joyValue1 > 30) || (joyValue2 > 30)){
		motor[ScissorLeft] = 100 * leftLiftCon;

	}else if((joyValue1 < -30) || (joyValue2 < -30)){

		if(SensorValue(LiftTouchSensor) == 0)
			motor[ScissorLeft] = -100 * leftLiftCon;
		else motor[ScissorLeft] = 0;

	}else{
		motor[ScissorLeft] = 0;
	}
}

void leftControl(int range, int joyValue1, int joyValue2){

	if(joy2Btn(DOWN_BTN)){
		tensionControl();
	}

	if((joyValue1 > 30) || (joyValue2 > 30)){
		motor[ScissorRight] = 100 * rightLiftCon;

	}else if((joyValue1 < -30) || (joyValue2 < -30)){

		if(SensorValue(LiftTouchSensor) == 0)
			motor[ScissorRight] = -100 * rightLiftCon;
        else motor[ScissorRight] = 0;

	}else{
		motor[ScissorRight] = 0;
	}
}

void liftControl(int range, int joyValue1, int joyValue2){


	if((joyValue1 > 30) || (joyValue2 > 30)){
		motor[ScissorLeft] = 100 * leftLiftCon;
		motor[ScissorRight] = 100 * rightLiftCon;

	}else if((joyValue1 < -30) || (joyValue2 < -30)){
		if(SensorValue(LiftTouchSensor) == 0){
			motor[ScissorLeft] = -100 * leftLiftCon;
			motor[ScissorRight] = -100 * rightLiftCon;

		}else{
			motor[ScissorLeft] = 0;
			motor[ScissorRight] = 0;
		}

	}else{
		motor[ScissorLeft] = 0;
		motor[ScissorRight] = 0;
	}
}

//--- ARM SERVO(S)

void armControl(){

	if(joy2Btn(RIGHT_BTN)){
		servo[ArmContRot] = 255;

 	}else if(joy2Btn(LEFT_BTN)){
 		servo[ArmContRot] = 0;

 	}else{
 		servo[ArmContRot] = 126;
 	}
}

//--- MAIN PROGRAM

task main(){

	initializeRobot();
	waitForStart(); //comment when not running FCS

    while(true){

        //get current joystick state with each update
		getJoystickSettings(joystick);

//--- DRIVE MOTORS (Controller #1)

        if(joy1Btn(L1) || joy1Btn(R1)){
            directDriveControl(50, joystick.joy1_y1, joystick.joy1_y2);

        }else if(joy1Btn(L2) || joy1Btn(R2)){
            directDriveControl(100, joystick.joy1_y1, joystick.joy1_y2);

        }else{ //default driving mode
            directDriveControl(75, joystick.joy1_y1, joystick.joy1_y2);
        }

//--- PULLEY LIFT (Controller #2)

        if(joy2Btn(L1) || joy2Btn(L2)){ //left-side modifiers
            leftControl(100, joystick.joy2_y1, joystick.joy2_y2);

        }else if(joy2Btn(R1) || joy2Btn(R2)){ //right-side modifiers
            rightControl(100, joystick.joy2_y1, joystick.joy2_y2);

        }else{ //default lifting mode
            liftControl(50, joystick.joy2_y1, joystick.joy2_y2);
        }

//--- TENSION CONTROL

        if((SensorValue(LiftTouchSensor) == 1) || (joy2Btn(DOWN_BTN))){
            tensionControl();
        }


//--- ARM CONTROL (Controller #2)

        armControl();
	}
}
