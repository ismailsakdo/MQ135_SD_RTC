#include <Wire.h>
#include <RTClib.h>
#include <SD.h>
#include <MQ135.h>

RTC_DS3231 rtc;
MQ135 mq135;
File myFile;

void setup() {
  //Initialize serial communication
  Serial.begin(115200);
  
  //Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (!rtc.initialized()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //Initialize MQ135 sensor
  mq135.setRZero(10);
  mq135.setRLoad(100000);
  
  // Initialize SD card
  if (!SD.begin(4)) {
    Serial.println("Initialization failed!");
    return;
  }
}

void loop() {
  //Read current date and time from RTC
  DateTime now = rtc.now();
  
  //Read CO2 value from MQ135 sensor
  float co2 = mq135.getPPM();
  
  //Open a new file on the SD card
  myFile = SD.open("data.txt", FILE_WRITE);
  
  //Write date, time and CO2 value to the file
  myFile.print(now.year(), DEC);
  myFile.print('/');
  myFile.print(now.month(), DEC);
  myFile.print('/');
  myFile.print(now.day(), DEC);
  myFile.print(' ');
  myFile.print(now.hour(), DEC);
  myFile.print(':');
  myFile.print(now.minute(), DEC);
  myFile.print(':');
  myFile.print(now.second(), DEC);
  myFile.print(",");
  myFile.println(co2);
  
  //Close the file
  myFile.close();
  
  //Print the values to the serial monitor
  Serial.print("Date and Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print("  ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Serial.print(" CO2 concentration: ");
  Serial.println(co2);
  
  //Delay for one minute
  delay(60000);
}
