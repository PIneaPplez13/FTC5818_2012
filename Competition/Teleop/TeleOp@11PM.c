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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c" //to handle Bluetooth messages

/* Here, defined are the button mappings for Logitech numbered controllers
according to the RobotC controller testing interface. It seems that in
RobotC, all buttons are assigned a number one less than that given on the
controller. Go figure?
*/

#define    UP_BTN 4
#define  DOWN_BTN 2
#define  LEFT_BTN 1
#define RIGHT_BTN 3

#define L1 5
#define L2 7
#define R1 6
#define R2 8

//we don't want the deadband to be too much
#define DEADBAND 10

//THESE VALUES STILL NEED TO BE CALCULATED
//#define ARM_EXTENDED_POS (255 * 4) //set arm servoTarget positions
//#define ARM_RETRACTED_POS 0

void initializeRobot(){

	nMotorEncoder[ScissorLeft] = 0; //reset motor encoders on scissor lift motors
  nMotorEncoder[ScissorRight] = 0;
  nMotorEncoder[LeftFront] = 0; //reset motor encoders on left drive motors
  nMotorEncoder[LeftRear] = 0;
  nMotorEncoder[RightFront] = 0; //reset motor encoders on right drive motors
  nMotorEncoder[RightRear] = 0;

  servo[ArmContRot] = 126; //initialize servo to starting position
}

int scaleForMotor(int joyValue, int maxSpeed){

	//motor speed in range -maxSpeed, +maxSpeed
	const int MAX_MOTOR_VALUE = maxSpeed;
	const float MAX_JOY_VALUE = 127.0;

	//check to see if joystick was accidentally nudged
	if(abs(joyValue) <= DEADBAND){
		return 0;
	}

	//calculate scaled value
	int direction = joyValue / abs(joyValue); //this results in either 1 or -1
	float ratio = pow(joyValue, 2) / pow(MAX_JOY_VALUE, 2);
	int scaledValue = (ratio * MAX_MOTOR_VALUE) * direction;

	return scaledValue; //motor speed given to motor
}

void armControl(){

	//manual arm control
	//if(joy2Btn(UP_BTN)){
		//servoTarget(ArmContRot) = ARM_EXTENDED_POS;
	//}else if(joy2Btn(LEFT_BTN)){
		//servoTarget(ArmContRot) = ARM_RETRACTED_POS;

	//automatic arm movement
	if(joy2Btn(RIGHT_BTN)){ //&& ServoValue(ArmContRot) < ARM_EXTENDED_POS){
		servo[ArmContRot] = 255;
 	}else if(joy2Btn(DOWN_BTN)){ //&& ServoValue(ArmContRot) > ARM_RETRACTED_POS){
 		servo[ArmContRot] = 0;
 	}else{
 		servo[ArmContRot] = 126;
 	}
}

void driveControl(int range){

	//motor speed and direction is directly translated from joystick position
	motor[LeftFront] = scaleForMotor(joystick.joy1_y1, range); //scale -range, +range
	motor[LeftRear]  = scaleForMotor(joystick.joy1_y1, range);
	motor[RightFront] = scaleForMotor(joystick.joy1_y2, range);
	motor[RightRear]  = scaleForMotor(joystick.joy1_y2, range);
}

void liftControl(int range){

	//motor direction is translated from joystick position
	//general motor speed it determined from modifier buttons
	motor[ScissorLeft] = scaleForMotor(joystick.joy2_y1, range); //scale -range to +range
	motor[ScissorRight] = scaleForMotor(joystick.joy2_y2, range);
}

task main(){

	initializeRobot();
	//waitForStart(); //comment whole line when testing and not running FCS

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
	  if(joy1Btn(L1) || joy1Btn(R1)){ //slow
	  	driveControl(15);
	  }else if(joy1Btn(L2) || joy1Btn(R2)){ //fast
	  	driveControl(100);
	  }else{
	  	driveControl(50); //default
	  }

	  //--- ARM CONTROL (Controller #2)
	  armControl();

	}
}
