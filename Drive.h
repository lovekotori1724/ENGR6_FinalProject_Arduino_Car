//downloaded from https://app.arduino.cc/sketches/1fd78c62-4b63-40cc-91d5-ef4814c58afd?view-mode=preview
//modified by Leo Zhao

// MOTOR LEFT PINS
#ifndef DRIVE_H
#define DRIVE_H

const int ENA = 11;
const int IN1 = 10;
const int IN2 = 12;

// MOTOR RIGHT PINS
const int ENB = 5;
const int IN3 = 4;
const int IN4 = 3;


// MOTOR PARAMETERS
// Minimum PWM value of analogWrite to spin motor when robot is on the ground.
const int MINIMUM_MOTOR_SPEED = 40;

int crashDetected = 0;
int throttle, steering;

void setupDrive() {
  // Set pins as input or output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}


// Configures the motor controller to stop the motors.
void motorBrake() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}

// Configures the motor controller to have the robot move forward.
void motorSetForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}

// Configures the motor controller to have the robot move backwards.
void motorSetBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Do not write the motor speeds on this function. It simply configures the motor controller.
}

void oneMotorForward(int EN)
{
  if(EN == ENA)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);    
  }
  if(EN == ENB)
  {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);  
  }
}

void oneMotorBackward(int EN)
{
  if(EN == ENA)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);   
  }
  if(EN == ENB)
  {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);  
  }
}

// Takes drive values and converts to PWM for motors
void drive(int throttle, int steering) {
  // Print Debug Info
    /*
    Serial.print("throttle: ");
    Serial.print(throttle);
    Serial.print("\tsteering: ");
    Serial.print(steering);
    */
    
    if (throttle == 0) {
      // If throttle is zero, don't move.
      motorBrake();
      return;
    }

    // Determine forwards or backwards.
    if (throttle > 0) {
      // Forward
      motorSetForward();
    } else {
      // Backward
      motorSetBackward();
    }

    // Map throttle to PWM range.
    int mappedSpeed = map(abs(throttle), 0, 512, MINIMUM_MOTOR_SPEED, 255);
    // Map steering to PWM range.
    int reducedSpeed = map(abs(steering), 0, 512, mappedSpeed, MINIMUM_MOTOR_SPEED);

    int leftMotorSpeed, rightMotorSpeed;
    if (steering > 0) {
      // Turn Right: reduce right motor speed
      leftMotorSpeed = mappedSpeed;
      rightMotorSpeed = reducedSpeed;
    } else {
      // Turn Left: reduce left motor speed
      leftMotorSpeed = reducedSpeed;
      rightMotorSpeed = mappedSpeed;
    }

    // Set motor speeds
    analogWrite(ENA, leftMotorSpeed);
    analogWrite(ENB, rightMotorSpeed);

    // Print Debug Info
    /*
    Serial.print("\tmappedSpeed: ");
    Serial.print(mappedSpeed);
    Serial.print("\treducedSpeed: ");
    Serial.print(reducedSpeed);
    Serial.print("\tleftMotorSpeed: ");
    Serial.print(leftMotorSpeed);
    Serial.print("\trightMotorSpeed: ");
    Serial.println(rightMotorSpeed);
    */
}

// This function gets the joystick values and uses them to drive the motors
void joystick(String id, String value) {

  if (id == "d1") {
    sscanf(value.c_str(), "%d,%d", &steering, &throttle);
    throttle -= 512;
    steering -= 512;
    //if a crash is detected, only allow the car to move backwards with the joystick
    //other wise, control the car to move in any direction using the joystick
    if(!(crashDetected && throttle > 0)) 
      drive(throttle, steering);
  }
}

#endif
