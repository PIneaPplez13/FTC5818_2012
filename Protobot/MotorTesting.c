
task main()
{
	motor[Left] = 100;
	motor[Right]= 100;

	wait1Msec(1000);

	motor[Left] = 0;
	motor[Right]= 0;

  wait1Msec(1000);

 	motor[Left] = -100;
	motor[Right]= -100;

	wait1Msec(1000);

	motor[Left] = 0;
	motor[Right]= 0;

	wait1Msec(1000);

	motor[Left] = 100;
	motor[Right]= -100;

	wait1Msec(1000);

	motor[Left] = 0;
	motor[Right]= 0;

	wait1Msec(1000);

	motor[Left] = -100;
	motor[Right]= 100;

	wait1Msec(1000);

	motor[Left] = 0;
	motor[Right]= 0;
}
