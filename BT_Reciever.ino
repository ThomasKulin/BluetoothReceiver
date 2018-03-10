#include <Servo.h>
int throttle, steering, button;
int prevthrottle = 1500;
int slowdown = 1500;
int prevvoltage = 0;
unsigned long starttime;
unsigned long vTimer;
int led = 10;
int interval = 500;
unsigned long blinktime;
int ledState = LOW;
int vUpdate = 1000;

short accelerate = 10;// acceleration modulator
short decelerate = 10;//braking modulator

Servo esc; //intitalize ESC

void setup() {
  starttime = millis();
  vTimer = millis()+vUpdate;
 // analogReference(DEFAULT); //forgot to include this in the circuit. oh well...
  pinMode(A5, INPUT);
  pinMode(A2, INPUT);
  pinMode(3, INPUT);
  pinMode(led, OUTPUT);
  pinMode(11, OUTPUT); //ESC singal
  pinMode(12, OUTPUT); //unassigned output
  pinMode(13, OUTPUT); //toggle
  digitalWrite(led, HIGH);
  blinktime = millis();
  Serial.begin(9600);
  esc.attach(11);
  esc.writeMicroseconds(0);
}
void loop() {
  if (Serial.available() > 0) {
    digitalWrite(led, HIGH);
    if (!esc.attached()) {
      esc.attach(11);
    }
    throttle = Serial.parseInt(); //parse throttle data from incoming serial 
    button = Serial.parseInt(); //parse toggle data from incoming serial
    if (Serial.read() == 'n') { //if incoming data is complete
      //if (throttle >= 800 && throttle <= 2200) {
        digitalWrite(13, HIGH);
        //Serial.print("b"); //start byte
        //int voltage = analogRead(A5);
        //Serial.print((int)(1.0647957189871569879086401003569*voltage)); //send voltage to controller
        //Serial.print("e"); //end byte
        esc.writeMicroseconds(throttle); //set motor speed to the recieved value
        prevthrottle = throttle;
        slowdown = throttle;
     // }
      starttime = millis();
      blinktime = millis();
    }
   if((millis()-vTimer) > vUpdate){
        int voltage = analogRead(A5);
        Serial.print((int)(voltage)); //send voltage to controller
        Serial.print('e');
        vTimer = millis();
    }
  }
  if ((millis() - starttime) > 500) { //after half a second has passed since throttle change
    if (prevthrottle > 1500 && slowdown != 1500) {
      esc.write(slowdown--); //Slow motor down eventually when no input is read
      delay(decelerate);
    }
    if (prevthrottle < 1500 && slowdown != 1500) {
      esc.write(slowdown++); //increase forward speed until motor stops
      delay(accelerate);
    }
  }
  if (millis() - blinktime > interval) {
    blinktime = millis();
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(led, ledState);
  }
}
