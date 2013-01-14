/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring it up!"

	Written by Joe Quigley
	A set of standard functions for reading sensors, as well as cdf/pdf calculations, mapping, etc.
	Debug lib ("stdbot_d.h") also contains screen-print funcs, debug funcs, file i/o funcs, and data processing.

	Revisions:
n		Rev 	Date		Notes
1		0.0		1/13/12	Initial version
*/

#ifndef STDBOT_H
#define STDBOT_H

//	includes =============================================
#include "hitechnic-eopd.h"
#include "hitechnic-irseeker-v2.h"
#include "lego-light.h"

//	typedefs =============================================
typedef struct	{
	int dir = -1;
	float rawStrength = -1.0;
	float strength = -1.0;
	int[5] _rawSensors = {-1, -1, -1, -1, -1};
}	tIRSeek;

//	vars =================================================
//	readIRSeeker()
bool _success = true;
int _largest = 0;
int _nextLargest = 0;

//	functions ============================================
int readEOPD(tSensors EOPD, bool raw)	{
	//	reads EOPD sensor
	//	@param	raw	whether to read raw val or not
	if(raw)	{
		return HTEOPDreadRaw(EOPD);
	}
	else	{
		return HTEOPDreadProcessed(EOPD);
	}
}

int readEOPD(tSensors EOPD)	{
	//	reads EOPD sensor
	return HTEOPDreadRaw(EOPD);
}

int readIRSeekDir(tSensors IRSeeker)	{
	//	reads IR seeker
	return HTIRS2readACDir(IRSeeker);
}

void readIRSeekDir(tSensors IRSeeker, tIRSeek &ir)	{
	//	reads IR seeker and stores it into ir.dir
	ir.dir = HTIRS2readACDir(IRSeeker);
}

bool readIRSeeker(tSensors IRSeeker, tIRSeek &ir)	{
	//	advanced read of the IR seeker
	//	spits out direction and strength info into tIRSeek ir
	//	direction: ir.dir
	//	raw strength: ir.rawStrength

	readIRSeekDir(IRSeeker, ir);

	_success = HTIRS2readAllACStrength(IRSeeker, ir._rawSensors[0], ir._rawSensors[1], ir._rawSensors[2], ir._rawSensors[3], ir._rawSensors[4]);

	if(!_success)	{
		return false;
	}

	for(int i = 0; i < 5; i++)	{
		if(ir._rawSensors[i] > _largest)	{
			_largest = ir._rawSensors[i];
		}
	}
	for(i = 0; i < 5; i++)	{
		if((ir._rawSensors[i] > _nextLargest) && (ir._rawSensors[i] != _largest)	{
			_nextLargest = ir._rawSensors[i];
		}
	}

	if((_largest - _nextLargest) < 126)	{
		ir.rawStrength = (_largest - _nextLargest) * 2;
	}
	else	{
		ir.rawStrength = _largest;
	}

	return true;
}

int readLegoLight(tSensors LightSensor)	{
	//	reads light sensor
	return LSvalRaw(LightSensor);
}

#endif
