#pragma config(Sensor, dgtl1,  ,               sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  ,               sensorQuadEncoder)
#pragma config(Motor,  port1,           motorleft,     tmotorVex393_HBridge, openLoop, encoderPort, dgtl1)
#pragma config(Motor,  port10,          motorright,    tmotorVex393_HBridge, openLoop, reversed, encoderPort, dgtl3)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	clearDebugStream();
	SensorType[in8] = sensorNone;
	wait1Msec(1000);
	SensorType[in8] = sensorGyro;//initializing gyro scope
	wait1Msec(3000);

	int cycles = 100;
	int n = 0;  //variable to kill the program after x number of cycles
	float adjustmentrate = 1; //variable for how much the robot reacts to it being off course
	int waittime = 500; //variable for how often the robot runs through the main loop

	float precision = 2; //how close is close enough?

	int dftspeed = 100; //Default speed for motors
	int lspeed = dftspeed;
	int rspeed = dftspeed; // Independant speeds for right and left

	float goalx = 100; // in cm.     goal distance for x, afterwhich while loop is broken
	float goaly = 0; // in cm.

	float distx = 0;
	float disty = 0;//distance travelled in the cycle
	float totalx = 0;
	float totaly = 0; // distance travelled total

	int newangle = 0;
	int oldangle = 0; //variables for data coming from gyro


	int oldrotatel = 0;
	int newrotatel = 0;
	int oldrotater = 0;
	int newrotater = 0; //variables for data coming in from encoders

	int rotatedistl = 0;
	int rotatedistr = 0; //Distance rotated this cycle

	float distl = 0;
	float distr = 0;//distance moved forward this cycle
	float avgdist = 0;//average distance travelled between both wheels

	float radiansangle = 0;// rotation angle from gyro in randians

	int end = 0; //Boolean value for whether end loop is done or not

	while (end == 0){
		//The following is all so that the computer can have a "mental map" of sorts as to where it is in relation to its start and end position
		oldangle = newangle;
		newangle = SensorValue[in8]; // Updating the current angle that the robot is pointing in, and saving the previous so calculations can be done

		oldrotatel = newrotatel;
		newrotatel = SensorValue[dgtl1];
		oldrotater = newrotater;
		newrotater = SensorValue[dgtl3];// updating encoder values

		rotatedistl = newrotatel - oldrotatel;
		rotatedistr = newrotater - oldrotater; //finding the degrees through which the wheels have rotated since the last check

		distl = rotatedistl / 360 * 26.5;
		distr = rotatedistr / 360 * 26.5; //finding the distance through which both wheels have rotated, by multiplying by the circumference(26.5 cm)
		avgdist = distl + distr;
		avgdist = avgdist / 2; //calculating the average distance through which both wheels/sides of the robot travel

		radiansangle = oldangle * 3.141592653589793 / 180.0; //calculating the angle in radians for trig
		distx = cos(radiansangle)*avgdist; //distance travelled in the x direction
		disty = sin(radiansangle)*avgdist; //distance travelled in the y direction
		totalx = totalx + distx;// summing total distance travelled in the x
		if (newangle < 0){
			totaly = disty * -1 + totaly; // The *-1 is nescessary so as to keep an indication of whether the robot is "above" or "below" the straight line, as this data is lost in the trig
		}
		else if (newangle > 0){
			totaly = disty + totaly;
		}

		motor(motorleft) = lspeed;
		motor(motorright) = rspeed;

		wait1Msec(waittime);
		//might want to do if statements for if distx < goalx and if distx > goalx

		//Now to actually make the motors run...   (implement straightline code)

		if (disty < goaly){ //So, if the robot is below the y axis and
			if(disty >= goaly - 2){ //if it is whithin 2 cm of the line, and
				if(newangle<=100){//if it is turning away from y axis, then
					lspeed = lspeed - adjustmentrate;
					rspeed = rspeed + adjustmentrate;//adjust the speed of motors so as to turn the robot
				}
			}
			else if (disty < -2){//But if the robot is further than 2 cm away, and
				if(newangle<=200){//if robot is turning away from y-axis, then
					lspeed = lspeed - 2*adjustmentrate;
					rspeed = rspeed + 2*adjustmentrate;//change speed at a greater rate,,, turning left
				}
			}
		}

		if (disty > goaly) {
			if (disty <= goaly + 2){
				if(newangle<=100){
					lspeed = lspeed + adjustmentrate;
					rspeed = rspeed - adjustmentrate;
				}
			}
			else if (disty > 2){
				if(newangle>=200){
					lspeed = lspeed + 2*adjustmentrate;
					rspeed = rspeed - 2*adjustmentrate;    //This section does the same as above.    turning right
				}
			}
		}


	}
		if (totalx >= goalx - 5){ //If the robot is approaching the target distance, it should
			dftspeed = 70; //reduce its speed down to 70, so that it can better judge its distance
			lspeed = 70;
			rspeed = 70; // This does get rid of any adjustments the robot was making, but thats just how it is
		}
		if(totalx >= goalx){
			motor(motorleft) = -10;
			motor(motorright) = -10;
			wait1Msec(50);
			motor(motorleft) = -15;
			motor(motorright) = -15;
			wait1Msec(50);
			motor(motorleft) = 0;
			motor(motorright) = 0; //Stopping the robot as quickly as possible

			while(end==0){
				if(totalx > goalx){
					motor(motorleft) = -25;
					motor(motorright) = -25;
					wait1Msec(waittime);
					if(goalx - precision < totalx < goalx){
						motor(motorright) = 5;
						motor(motorleft) = 5;
						wait1Msec(waittime);
						motor(motorleft)= 0;
						motor(motorright) = 0;
						if(goalx - precision < totalx < goalx){
							motor(motorleft) = 0;
							motor(motorright) = 0;
							end += 1;
						}
					}
				}
			}



	}




}
