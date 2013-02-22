#include "JoystickDriver.c"

task main()
{
	while(true)	{
		if(abs(joystick.joy1_y1) > 10)	{
			motor[motorB] = joystick.joy1_y1/3;
		}
		else	{
			motor[motorB] = 0;
		}
		if(abs(joystick.joy1_y2) > 10)	{
			motor[motorC] = joystick.joy1_y2/3;
		}
		else	{
			motor[motorC] = 0;
		}
	}
}
