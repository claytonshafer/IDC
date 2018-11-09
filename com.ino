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
}
void loop() {
    if(digitalRead(4) == HIGH) {
     char outgoing = 'c';
    Serial2.print(outgoing);
    digitalWrite(2, HIGH); //Yellow Lights
      }
    if(digitalRead(4) == LOW) {
      digitalWrite(2, LOW);
      }
    if(Serial2.available()){
    char incoming = Serial2.read();
    Serial.println(incoming);
    digitalWrite(3, HIGH); // Green Lights
    delay(1000);
    digitalWrite(3, LOW);
  }
  delay(50);
}
