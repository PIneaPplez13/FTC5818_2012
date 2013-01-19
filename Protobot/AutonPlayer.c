#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorRight,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     leftRear,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     rightFront,    tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ArmContRot,           tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)

#include "stdbot_d.h"

task main()
{
	initReadMode("auto.txt", 1024);

	for(int i = 0; i < 10; i++)	{
		PlayTone(523, 15);
		nextInt();
		nMotorEncoderTarget[leftFront] = nextInt();
		nMotorEncoderTarget[leftRear] = nextInt();
		nMotorEncoderTarget[rightFront] = nextInt();
		nMotorEncoderTarget[rightRear] = nextInt();
		motor[leftFront] = 100;
		motor[leftRear] = 100;
		motor[rightFront] = 100;
		motor[rightRear] = 100;

		while((nMotorRunState[leftFront] != runStateIdle) || (nMotorRunState[rightRear] != runStateIdle))	{}

		motor[leftFront] = 0;
		motor[leftRear] = 0;
		motor[rightFront] = 0;
		motor[rightRear] = 0;

		wait1Msec(100);
	}
}
