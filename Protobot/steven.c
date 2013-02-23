void getState()	{
	steven();
	//return "apple";
}

void steven()	{
	writeDebugStreamLine("steeeeven...");
	getState();
}

task main()
{
	getState();
}
