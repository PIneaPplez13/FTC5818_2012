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

#define loopi(x, y) (int i = x; i < y; i++)

tMotor mtrs[] = {LeftFront, LeftRear, RightFront, RightRear, ScissorLeft, ScissorRight};
int startPos = 1;

int Left[] = {100, 			100, 			-100, 		-100};
int Fwd[]	 = {100,			100,			100,			100};
int Right[]= {-100,			-100,			100,			100};

task main()
{

	if(startPos == 0)	{
		for loopi(0, 4)	{
			motor[mtrs[i]] = Left[i];
		}
	}
	else	{
		for loopi(0, 4)	{
			motor[mtrs[i]] = Right[i];
		}
	}

	while(abs(nMotorEncoder[LeftFront]) < 4500)	{};

	for loopi(0, 4)	{
		motor[mtrs[i]] = 0;
	}

}
