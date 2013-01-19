#include "stdbot_d.h"

task main()
{
	initReadMode("log.txt", 1024);

	for(int i = 0; i < 1000; i++)	{
		writeDebugStream("%i\t", nextInt());
		writeDebugStream("%f\n", (((nextInt())/1000.0)));
		nxtDisplayBigTextLine(1, "%i", nextInt());
		nxtDisplayBigTextLine(3, "%f", (((nextInt())/1000.0)));
		wait1Msec(40);
	}

	closeActiveFile();
}
