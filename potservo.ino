#include <Servo.h>
float min_read_level=10000;
float max_read_level=0;
boolean calibration_fail=true;
Servo myservo;
int prev_level=-1;
int servo_max=160;
int servo_min=0;
const int servoPin=9;
const int analogPin=A0;
const int servoDelay=100;
const int longServoDelay=500;

int translateLevelToPct(int lvl) {
  int pct =((lvl-min_read_level)* 100)/(max_read_level - min_read_level);
  if (pct <0) return 0;
  if (pct > 100) return 100;
  return pct;
}
int translatePctToAngle(int pct) {
  int angle =(servo_max - servo_min)* pct/100 + servo_min;
  if (angle < servo_min) return servo_min;
  if (angle > servo_max) return servo_max;
  return angle;
}
float angles[45]={};
float positions[45]={};
int count=0;

void calibrateServo() {
  int increment=4;
  myservo.attach(servoPin);
  digitalWrite(LED_BUILTIN, HIGH);
  int prev_read_level=-1;
  int read_level=-1;
  
  //Find Lower Bounds.
  int servo_angle=servo_min;
  int prev_servo_angle=servo_min;
  myservo.write(servo_angle);
  delay(longServoDelay);
  prev_read_level=analogRead(analogPin);
  read_level=prev_read_level;
  while (prev_read_level == read_level ){
    prev_servo_angle=servo_angle;
    servo_angle=servo_angle+1;
    myservo.write(servo_angle);
    delay(longServoDelay);
    prev_read_level=read_level;
    read_level=analogRead(analogPin);
  }
  min_read_level=prev_read_level;
  servo_min=prev_servo_angle;

  //Find Upper Bounds
  servo_angle=servo_max;
  prev_servo_angle=servo_max;
  myservo.write(servo_angle);
  delay(longServoDelay);
  prev_read_level=analogRead(analogPin);
  read_level=prev_read_level;

  while (prev_read_level == read_level ){

    prev_servo_angle=servo_angle;
    servo_angle=servo_angle-1;
    myservo.write(servo_angle);
    delay(longServoDelay);
    prev_read_level=read_level;
    read_level=analogRead(analogPin);
  }
  servo_max=prev_servo_angle;
  max_read_level=prev_read_level;

  myservo.detach();
}
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  calibrateServo();

}
void processResponse() {
  String incomingData = "";
    while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      incomingData += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
     
      int pos=translatePctToAngle(incomingData.toInt()) ;
      myservo.attach(9);
      delay(100);
      myservo.write(pos);
      delay(longServoDelay);
      myservo.detach();
      myservo.write(servo_min);  
      // clear the string for new input:
      incomingData = "";
    }
  }
}

// the loop routine runs over and over again forever:
void loop() {
  //TODO Temporary until debug;
  //Serial.println(analogRead(analogPin));
  // read the input on analog pin 0:
  
  int sensorValue = analogRead(A0);
  int sensor_level = translateLevelToPct(sensorValue);
  // print out the value you read:
  if (prev_level != sensor_level) {
    Serial.println(sensor_level);
    prev_level=sensor_level;
  }
  delay(100);  // delay in between reads for stability
  processResponse();

}
void blink() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
