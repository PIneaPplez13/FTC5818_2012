#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
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

#define nSTEPS 3

int steps[nSTEPS][4] = {{1000, 1000, 1000, 1000}, {1000, 1000, -1000, -1000}, {4000, 4000, 4000, 4000}};

tMotor mtrs[4] = {LeftRear, LeftFront, RightRear, RightFront};
tMotor *mtr = mtrs;

int nStep = 0;
int i = 0;
bool cont = true;
bool mtrRunning[4] = {true, true, true, true};
bool *run = mtrRunning;

task main()
{
	while(nStep < nSTEPS)	{
		motor[LeftFront	] = 100 * sgn(steps[nStep][0]);
		motor[LeftRear	] = 100 * sgn(steps[nStep][1]);
		motor[RightFront] = 100 * sgn(steps[nStep][2]);
		motor[RightRear	] = 100 * sgn(steps[nStep][3]);
		nMotorEncoder[LeftFront	] = 0;
		nMotorEncoder[LeftRear	] = 0;
		nMotorEncoder[RightFront] = 0;
		nMotorEncoder[RightRear	] = 0;
		i = 0;
		while(cont)	{
			for(i=0; i < 4; i++)	{
				if((*run + i) && (abs(nMotorEncoder[*(mtr+i)]) > abs(steps[nStep][i])))	{
					motor[*(mtr+i)] = 0;
					mtrRunning[i] = false;
				}
			}
			*run = mtrRunning;
			if(!mtrRunning[0] && !mtrRunning[1] && !mtrRunning[2] && !mtrRunning[3])	{
				cont = false;
			}
		}
		cont = true;
		nStep++;
		i = 0;
	}
}
