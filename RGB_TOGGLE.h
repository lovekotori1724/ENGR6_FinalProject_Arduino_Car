//downloaded from https://app.arduino.cc/sketches/1fd78c62-4b63-40cc-91d5-ef4814c58afd?view-mode=preview
//modified by Leo Zhao

#ifndef RGB_TOGGLE_H
#define RGB_TOGGLE_H

// Pin Declarations
const int redLED = 13;

//Variable Declarations
long previousTime = 0;
long currentTime = 0;
int flashInterval = 100;
int val = 0;

void setupLED()
{
  //set output pin
  pinMode(redLED, OUTPUT);
}

//if the switch is flipped, turn on or off the LED
void rgb_toggle(String id, String value) {
  if (id == "sw0") {
    if (value == "1") {
      digitalWrite(redLED, HIGH);
    } else if (value == "0") {
      digitalWrite(redLED, LOW);
    }
  }
}

//flash the LED every 100 ms
void flashLED()
{
  currentTime = millis();
  if(currentTime - previousTime > flashInterval)
  {
    val = (val == 0 ? 1 : 0);
    previousTime = currentTime;
  }
  digitalWrite(redLED, val);
}

//turn on the LED permanently
void turnOnLED()
{
  digitalWrite(redLED, HIGH);
}

//turn off the LED permanently
void turnOffLED()
{
  digitalWrite(redLED, LOW);
}
#endif
