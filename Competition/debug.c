typedef struct	{
	int count;
	int __maxCount;
	bool big;
	bool resetAfterOverflow;
} tDHandler;

void initDebug(tDHandler count, bool big = false, bool resetAfterOverflow = false)	{
	count.count = 1;
	count.big = big;
	count.resetAfterOverflow = resetAfterOverflow;

	if(count.big)	{
		count.__maxCount = 3;
	}
	else	{
		count.__maxCount = 6;
	}

	nxtDisplayTextLine(0, "FTC5818 DBG v0.5-");
}

void debug(char* text, tDHandler count)	{
	if(count.resetAfterOverflow && (count.count >= count.__maxCount))	{
		nxtEraseRect(0, 0, 99, 63);
		nxtDisplayTextLine(0, "FTC5818 DBG v0.5-");
		count.count = 1;
	}

	if(count.big)	{
		nxtDisplayBigTextLine(count.count, text);
		count.count += 1;
	}

	else	{
		nxtDisplayTextLine(count.count, text);
		count.count += 1;
	}
}
