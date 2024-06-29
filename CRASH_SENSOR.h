//Copied from week 13 lab
//Modified the code to work without using delay

#ifndef CRASH_SENSOR_H
#define CRASH_SENSOR_H

#include "Drive.h"
#include "RGB_TOGGLE.h"

const int trigPin=6; //Sensor Trig pin connected to Arduino pin 6
const int echoPin=9; //Sensor Echo pin connected to Arduino pin 9
float pingTime; //time for ping to travel from sensor to target and return
float targetDistance; //Distance to Target in inches
float speedOfSound=776.5; //Speed of sound in miles per hour when temp is 77 degrees.
long long previousTimeInMicroSeconds = 0;
long long currentTimeInMicroSeconds = 0;

//these variables check which if-loop in measureDistance() should be executed
//the program seems to work without these checks, but I kept them to make sure all if-loops are executed in order
int loop1 = 0;
int loop2 = 0;
int loop3 = 0;

//0 for joystick controlled driving, 1 for autonumous driving
int driveMode = 0;

void setupCrashSensor() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  previousTimeInMicroSeconds = micros();
  currentTimeInMicroSeconds = micros();
}

//the following function is modified to work without using delay
float measureDistance(){
  currentTimeInMicroSeconds = micros();
  digitalWrite(trigPin, LOW);
  //if it has been at least 2000 us since we write LOW to the trigPin and we are starting over, execute this loop
  if(currentTimeInMicroSeconds > previousTimeInMicroSeconds + 2000 && loop1 == 0) 
  {
    digitalWrite(trigPin, HIGH);
    loop1 = 1;
    loop2 = 0;
    loop3 = 1;
  }
  //if it has been at least 2015 us since we write LOW to the trigPin and we just finished loop 1, execute this loop
  if(currentTimeInMicroSeconds > previousTimeInMicroSeconds + 2015 && loop2 == 0) 
  {
    digitalWrite(trigPin, LOW);
    loop1 = 1;
    loop2 = 1;
    loop3 = 0;
  }
  //if it has been at least 2025 us since we write LOW to the trigPin and we just finished loop 2, execute this loop
  if(currentTimeInMicroSeconds > previousTimeInMicroSeconds + 2025 && loop3 == 0)
  {
    pingTime = pulseIn(echoPin, HIGH); //pingTime is presented in microceconds
    pingTime = pingTime/1000000; //convert pingTime to seconds by dividing by 1000000 (microseconds in a second)
    pingTime = pingTime/3600; //convert pingtime to hourse by dividing by 3600 (seconds in an hour)
    targetDistance = speedOfSound * pingTime; //This will be in miles, since speed of sound was miles per hour
    targetDistance = targetDistance/2; //Remember ping travels to target and back from target, so you must divide by 2 for actual target distance.
    targetDistance = targetDistance*63360; //Convert miles to inches by multipling by 63360 (inches per mile)
    previousTimeInMicroSeconds = currentTimeInMicroSeconds;
    //Serial.println(targetDistance); //for debugging

    //reset so we start at loop 1 again
    loop1 = loop2 = loop3 = 0;    
  } 

  return targetDistance;
}

//returns true if crash is detected, false otherwise
//I initially wanted to use the return value of this function to control the vehicle's motion
//However, I reazlied that I would be much easier to do so using a global variable, crashDetected
bool detectCrash()
{
  measureDistance();

  /*
  Serial.print("The target distance is ");
  Serial.println(targetDistance); //for debugging
  */

  //if targetDistance is less than 5 inch, flash LED, brake, set the variable crashDetected to 1, and return true
  if(targetDistance < 5)
  {
    flashLED();
    if(throttle > 0)
      motorBrake();
    crashDetected = 1;
    //Serial.println(crashDetected); //for debugging
    return true;
  }
  //if targetDistance is greater than or equal to 5 inch, turn on the LED if we are at autonomous driving mode 
  //or turn off the LED if we are at joystick driving mode,  set the variable crashDetected to 0, and return false
  (driveMode == 1 ? turnOnLED() : turnOffLED());
  crashDetected = 0;

  //Serial.println(crashDetected); //for debugging
  return false;
}
#endif