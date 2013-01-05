//	#pragmas
#pragma config(Sensor, S2, SensorMux, sensorI2CCustom)

//	includes
#include "hitechnic-sensormux.h"
#include "hitechnic-force.h"
#include "JoystickDriver.c"

//	#defines
#define HEAVY 500
#define FARCE msensor_S2_2

//	vars
int _force = 0;

task main()
{
	waitForStart();

	nxtDisplayTextLine(1, "Force sensor VAL");

	while(1==1)	{
		_force = HTFreadSensor(FARCE);
		nxtDisplayTextLine(2, "Force: %i", _force);

		nxtDisplayTextLine(4, "Probably: ");
		if(_force > HEAVY)	{
			nxtDisplayTextLine(5, "HOOVY");
		}
		else	{
			nxtDisplayTextLine(5, "LIGHT");
		}
	}
}
