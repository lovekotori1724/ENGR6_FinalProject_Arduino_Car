#ifndef LINE_TRACKING_H
#define LINE_TRACKING_H

//variable declaration
const int lineTracker1 = A1;
const int lineTracker2 = A2;
const int carSpeed = 100;
const double scale = 0.7;

void setupLineTracking() {
  // Set pins as input
  pinMode(lineTracker1, INPUT);
  pinMode(lineTracker2, INPUT);
}

//convert analog readings from the line trackers to digital 
//this is because I ran out off digital pins
int readLineTracker(int lineTracker)
{
  int val = analogRead(lineTracker);
  val = (val < 100 ? 0 : 1);
  return val;
}

//line tracking algorithm
void lineTracking()
{
  int value1;
  int value2;

  value1 = readLineTracker(lineTracker1);
  value2 = readLineTracker(lineTracker2);

  //for debugging
  /*
  Serial.print("lineTracker1 value: ");
  Serial.print(value1);
  Serial.print("  lineTracker2 value: ");
  Serial.println(value2);
  */

  //if a crash is detected stop the car, otherwise, process with the line tracking algorithm
  if(crashDetected)
  {
    motorBrake();
  }else{
    //if both sensors read 0 or 1, move forward
    //the speed for ENA is reduced because ENA spins faster than ENB
    if((value1 == 0 && value2 == 0)||(value1 == 1 && value2 == 1))
    {
      motorSetForward();
      analogWrite(ENB, carSpeed);
      analogWrite(ENA, carSpeed - 20);

    } else{
      //stop the car briefly if one sensor reads 0 and the other reads 1
      motorBrake();
      //if the left sensor reads 0 and the right sensor read 1
      //move left motor forward and right motor backward so the car turns right
      //the speeds are reduced by multiplying scale so the vehicle turns slowly
      if(value1 == 1 && value2 == 0)
      {
        oneMotorBackward(ENA);
        analogWrite(ENA, (carSpeed - 20)* scale);
        oneMotorForward(ENB);
        analogWrite(ENB, carSpeed* scale);
      }
      //if the left sensor reads 1 and the right sensor read 0
      //move left motor backward and right motor forward so the car turns left
      //the speeds are reduced by multiplying scale so the vehicle turns slowly
      if(value2 == 1 && value1 == 0)
      {
        oneMotorForward(ENA);
        analogWrite(ENA, (carSpeed - 20)* scale);
        oneMotorBackward(ENB);
        analogWrite(ENB, carSpeed* scale);
      }
    }
  }
}

#endif
