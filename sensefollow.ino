
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);
const int pingPin = 50;
unsigned int duration, inches;
Servo servoLeft;
Servo servoRight;
#include <SoftwareSerial.h>
#define Rx 14
#define Tx 15


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(500);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT); 
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
servoLeft.attach(12);
servoRight.attach(11);
Serial.begin(9600);
int intcount = 0;
bool go = true;
int location = 0;
while (go){
  int type = 0;
  int LeftQ = RCTime(47);
  int MidlQ = RCTime(49);
  int MidrQ = RCTime(51);
  int RightQ = RCTime(53);
  if (LeftQ > 50)
    {
    type = type + 1;
    }
  if (MidlQ > 50)
    {
    type = type + 2;
    }

  if (MidrQ > 50)
    {
     type = type + 4;
    }
  if (RightQ > 50)
    {
      type = type + 8;
    }
 switch (type) {
  case 0:
    //no line
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1600);
  break;
  case 1:
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1550);
  break;
  case 3:
  case 7:
  //left
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1400);
  break;
  case 2:
  case 4:
  case 6:
  //straight
  servoLeft.writeMicroseconds(1600);         
  servoRight.writeMicroseconds(1400);
  break;
  case 8:
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1450);
  break;
  case 12:
  case 14:
  // right
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1600);
  break;
  case 15:
  Serial.println(intcount);
  //intersection
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(1000);
  intcount++;
  Serial2.print(location); 
 Serial.println(heightfunction());
 Serial.println(colorfunction());
  if (heightfunction() < 200 && colorfunction() > 3500)
  {
    location = intcount;
    Serial.println(location);
    Serial.println("mark");   
  }
  else {
    
  }
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1400);
  delay(300);
  break;
  default:
  //nothing
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  break;
 }
 if (intcount == 5){
  go = false;
  Serial2.print("Block location: ");
  Serial2.print(location);
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  break;
 }
 delay(5);
}


}

void loop() {
  // put your main code here, to run repeatedly:

}

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

int colorfunction(void) {
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  return colorTemp;
}

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
