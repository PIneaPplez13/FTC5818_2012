//	#pragmas
#pragma config(Sensor, S2,,sensorI2CCustom)

//	includes
#include "hitechnic-force.h"
#include "hitechnic-sensormux.h"

//	#defines
#define HEAVY 500
#define FARCE msensor_S2_1

//	vars
int force = 0;

task main()
{
	nxtDisplayTextLine(1, "Force sensor VAL");

	while(1==1)	{
		force = HTFreadSensor(FARCE);
		nxtDisplayTextLine(2, "Force: %i", force);

		nxtDisplayTextLine(4, "Probably: ");
		if(force > HEAVY)	{
			nxtDisplayTextLine(5, "HOOVY");
		}
		else	{
			nxtDisplayTextLine(5, "LIGHT");
		}
	}
}
