void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int wait = random(250,750);
  int turnOn = 0;
  
  if (Serial.available()) 
  {
    // Expects an int.
    turnOn = Serial.parseInt();
    // FOR ACTUAL CODE:
    //    Gets an 11, 12, 13, 14 if want manual command
    //    Gets a 0XXXXYYYY if want automatic
    //    DO IF STATEMENT: 
    //      If < 20, treat as manual, do servostuff
    //      If > 10001 ((In C++ have X must be > 0, Y > 0, so can assume this))
    //         then break apart and do stuff

    // AUTO LOGIC:
    //    Value / 10000 == XXXX.YYYY, int, lose Y. Done.
    //    Value % 10000 == 0000YYYY, int, lost X. Done.
    //    NewVal / 1000 == X.xxx, int, done.
    //    NewVal /  100 == XX.xx, int, %10, done
    //    NewVal /   10 == XXX.x, int, %10, done
    //    NewVal %   10 == 000X, done.
    
    // If the angle to move to is greater than 0, we can move to it
    // If angle > 180, servo moves to 180
    if(turnOn == 1)
    {
        digitalWrite(13,HIGH);
        Serial.write("Light Turn On \n");
        delay(wait);
    } 
    if(turnOn == 0) 
    {
        digitalWrite(13,LOW);
        Serial.write("Light Turn Off \n");
        delay(wait);
    }
    if(turnOn == 11) 
    {
      while(true){
        digitalWrite(13,LOW);
        delay(wait);
        digitalWrite(13,HIGH);
        delay(wait);
        if (Serial.available()) 
          break;
      }
    }
  }
}
