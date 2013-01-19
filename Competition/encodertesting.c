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

task main()
{

	nMotorEncoder[LeftFront] = 0;
	nMotorEncoder[LeftRear] = 0;
	nMotorEncoder[RightFront] = 0;
	nMotorEncoder[RightRear] = 0;

	motor[LeftFront] = 100;
	motor[LeftRear] = 100;
	motor[RightFront] = 100;
	motor[RightRear] = 100;

	wait1Msec(1000);

	motor[LeftFront] = 0;
	motor[LeftRear] = 0;
	motor[RightFront] = 0;
	motor[RightRear] = 0;

	nxtDisplayTextLine(0, "  Left\tRight");
	nxtDisplayTextLine(1, "F %i\t%i", nMotorEncoder[LeftFront], nMotorEncoder[RightFront]);
	nxtDisplayTextLine(2, "R %i\t%i", nMotorEncoder[LeftRear], nMotorEncoder[RightRear]);
	nxtDisplayTextLine(4, "F- Front R- Rear");

	wait1Msec(5000);
}
