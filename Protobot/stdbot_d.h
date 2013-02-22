/*
	FTC TEAM 5818 "BOTTERING RAMS" 2012-2013 "Ring it up!"

	Written by Joe Quigley
	A set of standard functions for reading sensors, as well as cdf/pdf calculations, mapping, etc.
	This debug version also contains screen-print funcs, debug funcs, file i/o funcs, etc.

	Revisions:
n		Rev 	Date		Notes
1		0.0		1/13/12	Initial version
2		0.0		1/14/12	Added screen drawing/file i/o
3		0.1		1/15/12	Added pdf/cdf
4		0.2		1/19/12	Added scalers and readUltrasonic changed to return distance in inches, added readUltrasonicRaw to return raw val
5		0.3		2/15/13	Adding IRSeeker regression functions
7		0.4		2/18/13	Begun DMM
8		0.5		2/21/13 More work on DMM, moved to seperate file. Fixed IRSeeker regression functions, somewhat.
*/

#ifndef STDBOT_H
#define STDBOT_H

#define DEBUG true
#define STDBOT_VERSION 0.5

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
//int _largest = 0;
//int _nextLargest = 0;

//	timer
bool _end = false;
volatile int _msecs = 0;

//	file i/o
TFileHandle file;
TFileIOResult err;
int _val = 0;

//	display
int _count = 0;

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
//	SCALE FUNCS

int Map(int val, int lo, int hi, int rlo, int rhi)	{
	//	maps integer val of low lo and high hi to a range of rlo and rhi.
	val -= lo;
	val = ((((float)val/(hi-lo))*(rhi-rlo)) + rlo);
	return val;
}

float Map(float val, float lo, float hi, float rlo, float rhi)	{
	//	maps float val of low lo and high hi to a range of rlo and rhi.
	val -= lo;
	val = (((val/(hi-lo))*(rhi-rlo)) + rlo);
	return val;
}

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
	//	reads IR seeker direction
	return HTIRS2readACDir(IRSeeker);
}

void readIRSeekDir(tSensors IRSeeker, tIRSeek &ir)	{
	//	reads IR seeker direction and stores it into ir.dir
	ir.dir = HTIRS2readACDir(IRSeeker);
}

void readIRSeekerDist(tIRSeek &ir)	{
	//	reads the actual distance in inches from the IRSeeker.
	//	WARNING! Distances below 12 inches will NOT return the correct value!
	ir.strength = -0.00681066*ir.rawStrength + 1.9035848;
}

bool readIRSeeker(tSensors IRSeeker, tIRSeek &ir)	{
	//	advanced read of the IR seeker
	//	spits out direction and strength info into tIRSeek ir
	//	direction: ir.dir
	//	raw strength: ir.rawStrength
	//	strength: ir.strength

	HTIRS2setDSPMode(IRSeeker, DSP_1200);

	_success = HTIRS2readAllACStrength(IRSeeker, ir._rawSensors[0], ir._rawSensors[1], ir._rawSensors[2], ir._rawSensors[3], ir._rawSensors[4]);

	if(!_success)	{
		writeDebugStreamLine("ERROR: stdbot: oh noes! HTIRS2readAllACStrength() call failed. Is the IRSeeker working?");
		return false;
	}

	_success = HTIRS2readEnhanced(IRSeeker, ir.dir, ir.rawStrength);

	if(!_success)	{
		writeDebugStreamLine("ERROR: stdbot: oh noes! HTIRS2readEnhanced() call failed. Is the IRSeeker working?");
		return false;
	}

	readIRSeekerDist(ir);

	return true;
}

int readLegoLight(tSensors LightSensor)	{
	//	reads light sensor
	return LSvalRaw(LightSensor);
}

int readUltrasonicRaw(tSensors US)	{
	//	reads ultrasonic distance
	return USreadDist(US);
}

int readUltrasonic(tSensors US)	{
	//	reads distance in inches
	return (-6.504+(.5171*USreadDist(US)));
}

//	PROBABIlITY FUNCS

float normPDF(float z, float mu, float sigma)	{
	//	Calculates probability z in a normal curve with mean mu and standard deviation sigma
	if(z < mu)	{
		return Phi(z, mu, sigma)/2.0;
	}
	else	{
		z = (2*mu)-z;
		return Phi(z, mu, sigma)/2.0;
	}
}

float normCDF(float z, float mu, float sigma)	{
	//	Calculates probability z in a cumulative normal curve with mean mu and standard deviation sigma
	return Phi(z, mu, sigma);
}

//	TIMER FUNCS

void beginNewTimer(int ms)	{
	//	starts a new timer
	//	only one can be active at a time, _msecs should be considered volatile
	_msecs = ms;
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

//	Debugger Functions ===================================
//	FILE IO FUNCS

void initWriteMode(char* filename, int nBytes, bool deleteIfExists = true)	{
	//	opens a file for writing
	//	note that only one file may be open at a time
	if(deleteIfExists)	{
		Delete(filename, err);
	}
	OpenWrite(file, err, filename, nBytes);
}

void initReadMode(char* filename, int nBytes)	{
	//	opens a file for reading
	OpenRead(file, err, filename, nBytes);
}

void closeActiveFile()	{
	//	closes the active file
	Close(file, err);
}

int nextInt()	{
	//	reads next logical int
	ReadShort(file, err, _val);
	return _val;
}

void writeInt(int toWrite)	{
	//	writes an int
	WriteShort(file, err, toWrite);
}

//	DISPLAY FUNCS

void plot(int x, int y)	{
	nxtDrawLine(x, 0, x, y);
}

void plot(int y)	{
	nxtDrawLine(_count, 0, _count, y);
	_count++;
}

void setx(int x)	{
	_count = x;
}

int getx()	{
	return _count;
}

#endif
