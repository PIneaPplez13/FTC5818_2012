#pragma config(Sensor, S1,     IRSeeker,       sensorI2CCustom)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "stdbot_d.h"

tIRSeek ir;

task main()
{
	while(true)	{
		readIRSeeker(IRSeeker, ir);
		nxtDisplayTextLine(0, "IRSeek dist");
		nxtDisplayBigTextLine(1, "%f in", readIRSeekerDist(ir._rawSensors[2]));
	}
}
