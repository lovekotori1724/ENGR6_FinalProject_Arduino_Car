//downloaded from https://app.arduino.cc/sketches/1fd78c62-4b63-40cc-91d5-ef4814c58afd?view-mode=preview
//modified by Leo Zhao

// Include header files
#include "RGB_TOGGLE.h"  // Example 2: Toggle RGB On/Off
#include "DRIVE.h"       // Example 4: Control 2WD Robot
#include "LINE_TRACKING.h"
#include "EEPROM.h"
#include "CRASH_SENSOR.h"
#include "MPU.h"

// Include libraries
#include <SoftwareSerial.h>


// Declare RX and TX pins for bluetooth serial communication
const int rXPin = 7;
const int tXPin = 8;


//Object Initialization
SoftwareSerial BLESerial(rXPin, tXPin);  // Initialize serial channel for bluetooth

//struct for a Bluetooth message
struct BLEMessage {
  String id = "";
  String value = "";
  int example;
};

//get a bluetooth message from the app if there is one
struct BLEMessage getBLEMessage() {
  BLEMessage msg;
  // Read id and value if detect signature code 1
  if (BLESerial.available()) {
    int v = BLESerial.read();
    if (v == 1) {
      unsigned long startTime = millis();
      while (millis() - startTime < 1000) {
        if (BLESerial.available()) {
          v = BLESerial.read();
          if (v == 2) break;       // Move to next while loop
          msg.id.concat((char)v);  // Append character to id string
        }
      }
      while (millis() - startTime < 1000) {
        if (BLESerial.available()) {
          v = BLESerial.read();
          if (v == 3) break;          // Finish appending to message strings
          msg.value.concat((char)v);  // Append character to value string
        }
      }
    }
    msg.value.toLowerCase();     // Convert all letters in value to lowercase
    msg.value.replace(" ", "");  // Filter out all spaces in message value
  }
  // Print id and value if Serial exist
  /*if (Serial) {
    if (msg.id.length() > 0 && msg.value.length() > 0) {
      Serial.print("id:");
      Serial.print(msg.id);
      Serial.print(" ");
      Serial.print("value:");
      Serial.println(msg.value);
    }
  }
  */
  return msg;
}

//sets up every sensor from the .h files
void setup() {
  Serial.begin(9600);
  // Connect BLE serial port
  BLESerial.begin(9600);
  //BLESerial.write("AT+NAMELeoZhao");
  setupDrive();
  setupEEPROM();
  setupLineTracking();
  setupLED();
  setupCrashSensor();
  setupMPU();

  //readAccelData(); //not used in test drive
  //readMaxAccel(); //used after test drive to get max acceleration data from EEPROM
}


void loop() 
{
  //detect crash
  detectCrash();

  //gets a bluetooth message if there is one
  BLEMessage msg = getBLEMessage();
  //if switch is flipped, change the drive mode
  if(msg.id == "sw0")
  {
    driveMode = (driveMode == 0 ? 1 : 0);
    motorBrake();
  }

  //if the button is press, update Max Acceleration in the EEPROM
  if(msg.id == "b0")
  {
    writeMaxAccel();
  }  
  
  if(driveMode == 0)
  {
    //if the drive mode is joy-stick driving
    //control the car with a joystick
    if(msg.id == "d1")
      joystick(msg.id, msg.value);
  }else{
    //if the drive mode is autonomous, make the car follow a black line
    lineTracking();
  }
  //writeAccelData(); //not used
  //update max acceleration data
  updateMaxAccel();

}
