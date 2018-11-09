#include <Servo.h>
Servo servoLeft;
Servo servoRight;
void setup() {
  // put your setup code here, to run once:
servoLeft.attach(12);
servoRight.attach(11);
Serial.begin(9600);

}
//left = 47, centerl = 49, centerr = 51, right = 53
void loop() {
  // put your main code here, to run repeatedly:
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
  Serial.println(type);
  int counter = 0;
 switch (type) {
  case 0:
    //no line
   if (counter < 4)
    {
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1600);
  delay(5);
  counter++;
    }
    else
    {
      servoLeft.writeMicroseconds(1500);
      servoRight.writeMicroseconds(1500);
      delay(10000);
    }
  case 1:
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1550);
  break;
  case 3:
  case 7:
  //left
  servoLeft.writeMicroseconds(1400);
  servoRight.writeMicroseconds(1400);
  counter = 0;
  break;
  case 2:
  case 4:
  case 6:
  //straight
  servoLeft.writeMicroseconds(1600);         
  servoRight.writeMicroseconds(1400);
  break;
  case 8:
  counter = 0;
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1450);
  break;
  case 12:
  case 14:
  counter = 0;
  // right
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1600);
  break;
  case 15:
  //intersection
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1500);
  delay(1000);
  
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
 delay(5);
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
