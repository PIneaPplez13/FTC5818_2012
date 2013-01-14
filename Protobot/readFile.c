#include "stdbot_d.h"

task main()
{
	initReadMode("log.txt", 1024);

	for(int i = 0; i < 1000; i++)	{
		writeDebugStream("USDIST %i\n", (((float)nextInt())/1000.0));
		wait1Msec(10);
		writeDebugStream("CONSTD %i\n", nextInt());
		wait1Msec(10);
	}

	closeActiveFile();
}
