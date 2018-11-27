#include <SoftwareSerial.h>
void setup() {
  // put your setup code here, to run once:
Serial3.begin(9600);
  delay(100);
  Serial3.write(12);                 // Clear             
  Serial3.write(17);                 // Turn backlight on
  delay(5);                           // Required delay
  Serial3.print("Hello, world...");  // First line
  Serial3.write(13);                 // Form feed
  Serial3.print("from Parallax!");   // Second line
  Serial3.write(212);                // Quarter note
  Serial3.write(220);                // A tone
  delay(3000);                        // Wait 3 seconds
  Serial3.write(18);

}

void loop() {
  // put your main code here, to run repeatedly:
  
}
