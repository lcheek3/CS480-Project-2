

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
  y_servo.write(170);
  x_servo.write(90);
} 
 
void loop() 
{ 
  while(Serial.available() > 0){
    char command = Serial.read();
     if (command == 'A'){
        x_servo.write(Serial.parseInt());
        y_servo.write(Serial.parseInt());
        Serial.write(x_servo.read());
        Serial.write(y_servo.read());
     }
     if (command == 'M'){
        int in = Serial.parseInt();
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
