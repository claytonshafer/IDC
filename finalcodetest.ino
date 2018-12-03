
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <elapsedMillis.h>
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
const int pingPin = 50;
unsigned int duration, inches;
Servo servoLeft;
Servo servoRight;
int scores[] = {0,0,0,0,0};
#include <SoftwareSerial.h>
#define Rx 14
#define Tx 15 

void setup() {
  Serial.begin(9600); //monitor
  Serial2.begin(9600);//xbee
  Serial3.begin(9600);//LCD
  delay(500);
  pinMode(2, OUTPUT); //LED troubleshooting pin
  digitalWrite(2, LOW);
  servoLeft.attach(12);
  servoRight.attach(11);
  int intcount = 0; //intersection counter
  bool go = true; 
  int location = 0;

while(go) {
  int LeftQ = RCTime(47); //reads each QTI sensor and assigns its value to a variable
  int MidQ = RCTime(49);
  int RightQ = RCTime(51);
  int type = 0; //type is a variable which determines which action the robot should take
   if ((LeftQ > 50) || (LeftQ > 50 && MidQ > 50)) //Left
    {
    type = 1;
    }
  if (MidQ > 50 && LeftQ < 50 && RightQ < 50) //straight
    {
    type = 2;
    }

  if ((RightQ > 50) || (RightQ > 50 && MidQ > 50)) //right
    {
     type = 3;
    }
  if (RightQ > 50 && MidQ > 50 && LeftQ > 50) //intersection
    {
      type = 4;
    }
  if (RightQ < 50 && MidQ < 50 && LeftQ < 50) //no line- back up
  {
    type = 5;
  }
  switch (type) {
  case 1:
  //left
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1400);
  break;
  case 2:
  //straight
  servoLeft.writeMicroseconds(1600);         
  servoRight.writeMicroseconds(1400);
  break;
  case 3:
  // right
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1600);
  break;
  case 4:
  //intersection
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(1000);
  intcount++;
  Serial.println(heightfunction());
  Serial.println(colorfunction());
  if (heightfunction() < 200 && colorfunction() > 3500) //thresholds for the tall, blue block
  {
    location = intcount;
    digitalWrite(2, HIGH); //illuminates LED on top of bot for 2 sec (mostly for troubleshooting)
    delay(1000);
    digitalWrite(2, LOW);  
  }
  if (intcount < 5) { //moves the bot off the intersection to prevent double reads
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1400);
  delay(200);
  }
  break;
  case 5:
  //no line
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1600);
  delay(100);
  default:
  //nothing
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  break;
 }
if (intcount == 5){ //if 5 intersections have been read, stop moving
  go = false; //exit while loop
  Serial2.print(char(location + 96)); //send correct intersection number to the other bots
  scores[0] = location; //save correct intersection number to scores array

  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  break; 
 } 
 
  if (Serial2.available()){         //Checks to see if any other bots are transmitting letters. The letters are then categorized according 
    char incoming = Serial2.read(); // to the table shown in the appendix. 

    if ((incoming-'e') > 0 && (incoming-'e') <= 5){
      scores[2] = incoming - 'e'; 
 
    }
     else if ((incoming-'j') > 0 && (incoming-'j') <= 5){
      scores[1] = incoming - 'j'; 
   
    }
     else if ((incoming-'o') > 0 && (incoming-'o') <= 5){
      scores[3] = incoming - 'o'; 
    
    }
     else if ((incoming-'t') > 0 && (incoming-'t') <= 5){
      scores[4] = incoming - 't'; 
     
    }
  }
 delay(10);
}
}


void loop() {
  // put your main code here, to run repeatedly:

   if (Serial2.available()){        //Checks to see if any other bots are transmitting letters. The letters are then categorized according 
    char incoming = Serial2.read(); // to the table shown in the appendix. This code is repeated because otherwise the bot will not collect
                                    //data once it has stopped moving
    if ((incoming-'e') > 0 && (incoming-'e') <= 5){
      scores[2] = incoming - 'e'; 
   
    }
     else if ((incoming-'j') > 0 && (incoming-'j') <= 5){
      scores[1] = incoming - 'j'; 
  
    }
     else if ((incoming-'o') > 0 && (incoming-'o') <= 5){
      scores[3] = incoming - 'o'; 
     
    }
     else if ((incoming-'t') > 0 && (incoming-'t') <= 5){
      scores[4] = incoming - 't'; 
     
     }
      Serial3.write(12); //Displays the updated scores array on the serial LCD display
      Serial3.print(scores[0]);
      Serial3.print(scores[1]);
      Serial3.print(scores[2]);
      Serial3.print(scores[3]);
      Serial3.print(scores[4]);
 
    
  }
}

//This section contains functions to take readings from the three sensors

//Reads the value of the ping)) distance sensor. This value will be used to determine the height of the block
int heightfunction(void) {
  pinMode(pingPin, OUTPUT);          // Set pin to OUTPUT
  digitalWrite(pingPin, LOW);        // Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);       // Start ranging
  delayMicroseconds(5);              //   with 5 microsecond burst
  digitalWrite(pingPin, LOW);        // End ranging
  pinMode(pingPin, INPUT);           // Set pin to INPUT
  duration = pulseIn(pingPin, HIGH); // Read echo pulse
  inches = duration / 74 / 2;        // Convert to inches
  return duration;
}


//Reads the value of the color sensor. This value will be used to determine whether the water is clean
int colorfunction(void) {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  return colorTemp;
}


//Reads the value of one of the qti sensors. These will be used to detect the color of the mat below the bot, which is necessary for line following
long RCTime(int sensorIn){
   long duration = 0;
   pinMode(sensorIn, OUTPUT);     // Make pin OUTPUT
   digitalWrite(sensorIn, HIGH);  // Pin HIGH (discharge capacitor)
   delay(1);                      // Wait 1ms
   pinMode(sensorIn, INPUT);      // Make pin INPUT
   digitalWrite(sensorIn, LOW);   // Turn off internal pullups
   while(digitalRead(sensorIn)){  // Wait for pin to go LOW
      duration++;
   }
   return duration;
}
