#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     scissorLeft,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     scissorRight,  tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     leftRear,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     rightRear,     tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     rightFront,    tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    arm,                  tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,								tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

typedef struct {
	string name;
	int nSteps;
	int step[64][4];
} tStepArray;

tStepArray stp;
bool cont = true;
int _nSteps;

int calcMotorDrive(int joy, int adj)	{
	return((int)((joy/127.0)*adj));
}

void init()	{
	nxtDisplayCenteredTextLine(0, "STP FINDER");
	stp.name = "Autonomus";
}

void update()	{
	if(joy1Btn(1))	{
		nMotorEncoder[leftFront] = 0;
		nMotorEncoder[leftRear] = 0;
		nMotorEncoder[rightFront] = 0;
		nMotorEncoder[rightRear] = 0;

		nxtDisplayCenteredTextLine(7, "ENC reset.");
	}
	else if(joy1Btn(2))	{
		stp.step[stp.nSteps][0] = nMotorEncoder[leftFront];// = 0;
		stp.step[stp.nSteps][1] = nMotorEncoder[leftRear];// = 0;
		stp.step[stp.nSteps][2] = nMotorEncoder[rightFront];// = 0;
		stp.step[stp.nSteps][3] = nMotorEncoder[rightRear];// = 0;
		nxtDisplayCenteredTextLine(7, "STP %i/64", stp.nSteps);
		writeDebugStream("New step registered: %i/64", stp.nSteps);
		writeDebugStream("\tLeft Front:\t%i\n\tLeft Rear:\t%i", stp.step[stp.nSteps][0], stp.step[stp.nSteps][1]);
		writeDebugStream("\tRightFront:\t%i\n\tRightRear:\t%i", stp.step[stp.nSteps][2], stp.step[stp.nSteps][3]);
		stp.nSteps++;
	}
	else if(joy1Btn(3))	{
		nMotorEncoder[scissorLeft] = 0;
		nMotorEncoder[scissorRight] = 0;

		nxtDisplayCenteredTextLine(7, "ENC2 reset.");
	}
	else if(joy1Btn(4))	{
		writeDebugStream("Scissor Motors:\n\tScissorLeft:\t%i\n\tScissorRight:\t%i", nMotorEncoder[scissorLeft], nMotorEncoder[scissorRight]);
	}

	motor[leftRear] = calcMotorDrive(joystick.joy1_y1, 100);
	motor[leftFront] = calcMotorDrive(joystick.joy1_y1, 100);
	motor[rightRear] = calcMotorDrive(joystick.joy1_y2, 100);
	motor[rightFront] = calcMotorDrive(joystick.joy1_y2, 100);

	if(joy1Btn(10))	{
		cont = false;
	}
}

task main()
{
	for(int i = 0; i < 7; i++)	{
		nxtDisplayClearTextLine(i);
	}

	nNxtExitClicks = 2;
	while(cont)	{
		getJoystickSettings(joystick);
		update();
	}

	for(int i = 0; i < 7; i++)	{
		nxtDisplayClearTextLine(i);
	}

	while(nNxtButtonPressed != 0)	{
		if(nNxtButtonPressed == 3)	{
			nxtDisplayString(0, "STEP %i/%i", _nSteps, stp.nSteps);
			nxtDisplayString(1, "ENC1: %i", stp.step[_nSteps][0]);
			nxtDisplayString(2, "ENC2: %i", stp.step[_nSteps][1]);
			nxtDisplayString(3, "ENC3: %i", stp.step[_nSteps][2]);
			nxtDisplayString(4, "ENC4: %i", stp.step[_nSteps][3]);
		}
		else if(nNxtButtonPressed == 1)	{
			_nSteps++;
		}
		else if(nNxtButtonPressed == 2)	{
			_nSteps--;
		}
	}
}
