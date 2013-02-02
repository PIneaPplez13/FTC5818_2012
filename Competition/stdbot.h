/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring it up!"

	Written by Joe Quigley
	A set of standard functions for reading sensors, as well as cdf/pdf calculations, mapping, etc.
	Debug lib ("stdbot_d.h") also contains screen-print funcs, debug funcs, file i/o funcs, etc.

	Revisions:
n		Rev 	Date		Notes
1		0.0		1/13/12	Initial version
2		0.0		1/14/12	Added screen drawing/file i/o
3		0.1		1/15/12	Added pdf/cdf
*/

#ifndef STDBOT_H
#define STDBOT_H

//	includes =============================================
#include "hitechnic-eopd.h"
#include "hitechnic-irseeker-v2.h"
#include "lego-light.h"
#include "lego-ultrasound.h"
#include "stats.h"

//	typedefs =============================================
typedef struct	{
	int dir;
	float rawStrength;
	float strength;
	int _rawSensors[5];
}	tIRSeek;

//	vars =================================================
//	readIRSeeker()
bool _success = true;
int _largest = 0;
int _nextLargest = 0;

//	timer
bool _end = false;
volatile int _msecs = 0;

//	tasks ================================================
//	TIMER TASK

task timer()	{
	//	timer task
	//	keeps track of time, _msecs should be considered volatile
	while(!_end)	{
		wait1Msec(1);
		_msecs--;
		if(_msecs < 0)	{
			_end = true;
		}
		abortTimeslice();
	}
	_end = false;
	_msecs = 0;
}

//	functions ============================================

//	SENSOR FUNCS

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

	_largest = 0;
	_nextLargest = 0;

	for(int i = 0; i < 5; i++)	{
		if(ir._rawSensors[i] > _largest)	{
			_largest = ir._rawSensors[i];
		}
	}
	for(int i = 0; i < 5; i++)	{
		if((ir._rawSensors[i] > _nextLargest) && (ir._rawSensors[i] != _largest))	{
			_nextLargest = ir._rawSensors[i];
		}
	}

	if((_largest - _nextLargest) < 126 && ((_largest > 126) || (_nextLargest > 126)))	{
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

int readUltrasonic(tSensors US)	{
	//	reads ultrasonic distance
	return USreadDist(US);
}

//	PROBABIlITY FUNCS

float normPDF(float z, float mu, float sigma)	{
	//	Calculates probability z in normal curve
	if(z < mu)	{
		return Phi(z, mu, sigma)/2.0;
	}
	else	{
		z = (2*mu)-z;
		return Phi(z, mu, sigma)/2.0;
	}
}

float normCDF(float z, float mu, float sigma)	{
	//	Calculates probability z in cumulative normal curve
	return Phi(z, mu, sigma);
}

//	TIMER FUNCS

void beginNewTimer(int ms)	{
	//	starts a new timer
	//	only one can be active at a time, _msecs should be considered volatile
	_msecs = ms;
	_end = false;
	StartTask(timer, 8);
}

void endActiveTimer()	{
	//	ends the timer
	_end = true;
}

int getElapsed()	{
	//	returns _msecs
	//	warning! _msecs should be considered volatile
	return _msecs;
}

#endif
