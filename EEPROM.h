//This code is copied from project 2
//modified by Leo Zhao

#ifndef EEMPROM_H
#define EEMPROM_H

#include <Wire.h>

#include "MPU.h"

#define disk1 0x50 //Address of 24LC256 eeprom chip

int max = 1350; //max set of data
int count = 0; //records how many sets of data have been written
unsigned int address = 0; //starting eeaddress

//to keep track of accel data
float maxX = 0; 
float maxY = 0; 
float maxZ = 0; 
long long currentTimeEEPROM, previousTimeEEPROM; //to keep track of time

void setupEEPROM()
{
  Wire.begin();
  currentTimeEEPROM = millis(); 
  previousTimeEEPROM = millis();
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data
)
{
Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); // MSB
Wire.write((int)(eeaddress & 0xFF)); // LSB
Wire.write(data);
Wire.endTransmission();
//delay(5);
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress )
{
byte rdata = 0xFF;
Wire.beginTransmission(deviceaddress);
Wire.write((int)(eeaddress >> 8)); // MSB
Wire.write((int)(eeaddress & 0xFF)); // LSB
Wire.endTransmission();
Wire.requestFrom(deviceaddress,1);
if (Wire.available()) rdata = Wire.read();
return rdata;
}
//not needed for this program
void writeIntEEPROM(int deviceaddress, unsigned int eeaddress, int data)
{
byte byte1 = 0xFF;
byte byte2;
byte1 &= data;
byte2 = data>>8;
writeEEPROM(deviceaddress, eeaddress, byte1);
writeEEPROM(deviceaddress, eeaddress+1, byte2);
}
//not needed for this program
int readIntEEPROM(int deviceaddress, unsigned int eeaddress )
{
byte byte1 = readEEPROM(deviceaddress, eeaddress);
byte byte2 = readEEPROM(deviceaddress, eeaddress+1);
return (int)byte1 + (int)(byte2<<8);
}
//breaks a float into an array of bytes
void writeFloatEEPROM(int deviceaddress, unsigned int eeaddress, float data)
{
byte* p = (byte*)(void*)&data;
for (int i = 0; i < sizeof(data); i++) {
  writeEEPROM(deviceaddress, eeaddress + i, *p++);
}
}
//convert an array of bytes read from EEPROM to a float, then return it
float readFloatEEPROM(int deviceaddress, unsigned int eeaddress )
{
  float floatValue = 0.0;
  byte* p = (byte*)(void*)&floatValue;
  for (int i = 0; i < sizeof(floatValue); i++) {
    *p++ = readEEPROM(deviceaddress, eeaddress + i);
  }
  return floatValue;
}
//this function writes acceleration and gyro data to EEPROM every 500 ms
//this function is not used during the test drive
void writeAccelData()
{
  currentTimeEEPROM = millis();
  if(count < max && (currentTimeEEPROM - previousTimeEEPROM > 500))
  {
    previousTimeEEPROM = currentTimeEEPROM;
    recordAccelRegisters();
    recordGyroRegisters();
    writeFloatEEPROM(disk1, address, gForceX);
    address+=4;
    writeFloatEEPROM(disk1, address, gForceY);
    address+=4;
    writeFloatEEPROM(disk1, address, gForceZ);
    address+=4; 
    writeFloatEEPROM(disk1, address, rotX);
    address+=4;
    writeFloatEEPROM(disk1, address, rotY);
    address+=4;
    writeFloatEEPROM(disk1, address, rotZ);
    address+=4; 
    //printData();//for debugging
    count++;   
  }
}
//this function read all the acceleration and gyro data from EEPROM
//this function is not used during the test drive
void readAccelData()
{
  address = 0;
  for(int i = 0; i < max; i++)
  {
    gForceX = readFloatEEPROM(disk1, address);
    address += 4;
    gForceY = readFloatEEPROM(disk1, address);
    address += 4;
    gForceZ = readFloatEEPROM(disk1, address);
    address += 4;
    rotX = readFloatEEPROM(disk1, address);
    address += 4;
    rotY = readFloatEEPROM(disk1, address);
    address += 4;
    rotZ = readFloatEEPROM(disk1, address);
    address += 4;
    printData();
  }
}
//this function keeps track of the max X acceleration, max Y acceleration, and max Z acceleration
void updateMaxAccel()
{

  recordAccelRegisters();
  if(abs(gForceX) > maxX) 
  {
    maxX = abs(gForceX);
  }
  if(abs(gForceY) > maxZ) 
  {
    maxY = abs(gForceY);
  }
  if(abs(gForceZ) > maxZ) 
  {
    maxZ = abs(gForceZ);
  }
}

//this function updates the max X acceleration, max Y acceleration, and max Z acceleration in EEPROM 
void writeMaxAccel()
{
  address = 0;
  writeFloatEEPROM(disk1, address, maxX);
  writeFloatEEPROM(disk1, address + 4, maxY);
  writeFloatEEPROM(disk1, address + 8, maxZ);
}
//this function reads the max X acceleration, max Y acceleration, and max Z acceleration from the EEPROM
void readMaxAccel()
{
  address = 0;
  float maxX, maxY, maxZ;
  maxX = readFloatEEPROM(disk1, address);
  maxY = readFloatEEPROM(disk1, address + 4);
  maxZ = readFloatEEPROM(disk1, address + 8);

  Serial.print("\nMax frontal acceleration (g): ");
  Serial.println(maxY);
  Serial.print("Max side acceleration (g): ");
  Serial.println(maxZ);
  Serial.print("Max vertical acceleration (g): ");
  Serial.println(maxX);  

}
#endif
