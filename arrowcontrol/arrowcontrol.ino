/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo y_servo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
Servo x_servo;
 
int pos = 0;    // variable to store the servo position 
const int Y_PIN = 9;
const int X_PIN = 8;
 
void setup() 
{ 
  Serial.begin(9600);
  y_servo.attach(Y_PIN);  // attaches the servo on pin 9 to the servo object 
  x_servo.attach(X_PIN);
  y_servo.write(90);
  x_servo.write(90);
} 
 
void loop() 
{ 
  while(Serial.available() > 0){
    char command = Serial.read();
    if (command == 'A'){
      int ypos = Serial.parseInt();
      int xpos = Serial.parseInt();
      Serial.write(command);
      y_servo.write(ypos);
      x_servo.write(xpos);
    }
    if (command == 'M'){
      char in = Serial.parseInt();
      Serial.write(in);
      if (in == 1){
       y_servo.write(y_servo.read()-5);
      }
      if (in == 2){
       y_servo.write(y_servo.read()+5);
      }
      if (in == 3){
       x_servo.write(x_servo.read()-5);
      }
      if (in == 4){
        x_servo.write(x_servo.read()+5);
      }
    }
  }  
} 

