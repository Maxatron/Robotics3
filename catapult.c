#pragma config(Sensor, dgtl1,  encoderleft,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  encoderright,   sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  encoderc,       sensorQuadEncoder)
#pragma config(Motor,  port1,           motorleft,     tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port8,           catapult2,     tmotorVex393_MC29, openLoop, encoderPort, dgtl5)
#pragma config(Motor,  port9,           catapult,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          motorright,    tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	clearDebugStream();
	bool store = false;
	bool fwd = true;
	int cspeed = 8; //variable for the default speed of the catapult
	int k = 1;


	while(1==1){
		if(fwd == true){
			k = 1;
		}
		else if(fwd == false){
			k = -1;
		}

		if(SensorValue[Btn7L] == 1){
			fwd = true;
		}
		else if(SensorValue[Btn7R] == 1){
			fwd = false;
		}

		motor(motorleft) = k * vexRT[Ch2] / 2;
		motor(motorright) = k * vexRT[Ch3] / 2; //moving the main chasis

		if (store==true){//boolean value for if the robot should be holding a football in storage
			motor(catapult) = cspeed;
			motor(catapult2) = cspeed;
			if(SensorValue[dgtl5]>-33){
				cspeed += 1;
			}

			else if(SensorValue[dgtl5]< -40){
				cspeed -= 1;
			}



		}

		if(vexRT[Btn5D] == 1){ // Button to raise scooper for storage
			store = true;
		}


		if(vexRT[Btn5U] == 1){ // Button to lower scooper from raised position
			store = false;
			motor[catapult] = -30;
			motor[catapult2] = -30;
			wait1Msec(500);
			motor[catapult] = 0;
			motor[catapult2] = 0;
		}

		if(vexRT[Btn6D] == 1){ // Launch program
			motor(motorleft) = -10;
			motor(motorright) = -10;  //kills the motors so that the robot isnt moving uncontrollably while firing
			wait1Msec(80);
			motor(motorleft) = 0;
			motor(motorright) = 0;

			motor[catapult] = 127;
			motor[catapult2] = 127;
			wait1Msec(750);   //fires catapult
			motor[catapult] = -50;
			motor[catapult2] = -50;
			wait1Msec(500);  //brings catapult back
			motor[catapult] = 0;
			motor[catapult2] = 0;
		}


	}



}
