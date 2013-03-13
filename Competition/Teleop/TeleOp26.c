#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     ,               sensorHiTechnicTouchMux)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     ScissorLeft,   tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     ScissorRight,  tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     LeftRear,      tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     LeftFront,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     RightRear,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     RightFront,    tmotorTetrix, PIDControl, encoder)
#pragma config(Servo,  srvo_S1_C4_1,    ARM,                  tServoStandard)
#pragma config(Servo,  srvo_S1_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                           Tele-Operation Mode Code Template
//
// This file contains a template for simplified creation of an tele-op program for an FTC
// competition.
//
// You need to customize two functions with code unique to your specific robot.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                    initializeRobot
//
// Prior to the start of tele-op mode, you may want to perform some initialization on your robot
// and the variables within your program.
//
// In most cases, you may not have to add any code to this function and it will remain "empty".
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void initializeRobot()
{
  // Place code here to sinitialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

  return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                         Main Task
//
// The following is the main code for the tele-op robot operation. Customize as appropriate for
// your specific robot.
//
// Game controller / joystick information is sent periodically (about every 50 milliseconds) from
// the FMS (Field Management System) to the robot. Most tele-op programs will follow the following
// logic:
//   1. Loop forever repeating the following actions:
//   2. Get the latest game controller / joystick settings that have been received from the PC.
//   3. Perform appropriate actions based on the joystick + buttons settings. This is usually a
//      simple action:
//      *  Joystick values are usually directly translated into power levels for a motor or
//         position of a servo.
//      *  Buttons are usually used to start/stop a motor or cause a servo to move to a specific
//         position.
//   4. Repeat the loop.
//
// Your program needs to continuously loop because you need to continuously respond to changes in
// the game controller settings.
//
// At the end of the tele-op period, the FMS will autonmatically abort (stop) execution of the program.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

task main()
{
  initializeRobot();

  //waitForStart();   // wait for start of tele-op phase

  while (true)
  {
	  ///////////////////////////////////////////////////////////
	  //--- DRIVE MOTORS - CONTROLLER 1
		getJoystickSettings(joystick);
	  // SLOW modifier
	 	if(joy1Btn(6) || joy1Btn(7)){
	  	motor[LeftFront] = ceil(joystick.joy1_y1 * 0.39); //scale -50 to +50
	  	motor[LeftRear]  = ceil(joystick.joy1_y1 * 0.39);
	  	motor[RightFront] = ceil(joystick.joy1_y2 * 0.39);
	  	motor[RightRear]  = ceil(joystick.joy1_y2 * 0.39);
	  	//nxtDisplayTextLine(0, "forward slow");

		// FAST modifier
		}else if(joy1Btn(4) || joy1Btn(5)){
			motor[LeftFront] = joystick.joy1_y1 * 0.78; //scale -100 to +100
	  	motor[LeftRear]  = joystick.joy1_y1 * 0.78;
	  	motor[RightFront] = joystick.joy2_y1 * 0.78;
	  	motor[RightRear]  = joystick.joy2_y1 * 0.78;
	  	//nxtDisplayTextLine(0, "forward fast");

		// NO modifier
		}else{
			motor[LeftFront] = ceil(joystick.joy1_y1 * 0.58); //scale -75 to +75
	  	motor[LeftRear]  = ceil(joystick.joy1_y1 * 0.58);
	  	motor[RightFront] = ceil(joystick.joy1_y2 * 0.58);
	  	motor[RightRear]  = ceil(joystick.joy1_y2 * 0.58);
	  	//nxtDisplayTextLine(0, "no forward");
		}

		if(joy1Btn(3)){
			if(joy1Btn(6) || joy1Btn(7)){ //modifier
				motor[ScissorLeft] = 50;
				motor[ScissorRight] = 50;
			}else{                        //no modifier
				motor[ScissorLeft] = 100;
				motor[ScissorRight] = 100;
				//nxtDisplayTextLine(0, "lift raising");
			}
		// hat switch DOWN
		}else if(joy1Btn(1)){
			if(joy1Btn(6) || joy1Btn(7)){ //modifier
				motor[ScissorLeft] = 50;
				motor[ScissorRight] = 50;
			}else{                        //no modifier
				motor[ScissorLeft] = -100;
				motor[ScissorRight] = -100;
				//nxtDisplayTextLine(0, "lift lowering");
			}
		// NO hat switch
		}else{
			motor[ScissorLeft] = 0;
			motor[ScissorRight] = 0;
			//nxtDisplayTextLine(0, "no lift");
		}

    // Insert code to have servos and motors respond to joystick and button values.

    // Look in the ROBOTC samples folder for programs that may be similar to what you want to perform.
    // You may be able to find "snippets" of code that are similar to the functions that you want to
    // perform.
  }
}