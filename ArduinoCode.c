// This code is an arduino code to run a small self driving car through an obstacle course. This code would detect obstacles and change directions based on it

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

#define TRIG_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 300
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int leftDistance;
int rightDistance;
int distance;
int duration;
int speeds;
Servo servo_3;

// Motor A
int enA = 6;
int in1 = 9;
int in2 = 8;

// Motor B
int enB = 5;
int in3 = 7;
int in4 = 10;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //Set the LCD I2C address
//---------------------------------------------------------------------------------------
void setup()
{
  //servo + lcd + sensor
  servo_3.attach(3);
  Serial.begin(9600); // Starts the serial communication
  lcd.begin(16, 2); //start the lcd
  lcd.clear(); //clear the lcd
  servo_3.write(90); //put the servo to 90 degrees

  //enable pins for motor
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}
//--------------------------------------------------------------
void loop()
{
  straight();

  //servo
  servo_3.write(90); //sensor straight
  delay(90);

  distance = readPing();

  //lcd code
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(distance);
  lcd.setCursor(0, 1);
  lcd.print("cm");

  if (distance < 30 && distance > 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    delay(20);
    directionchoose();

  }
  delay(100);

}
//--------------------------------------------------------------------
void directionchoose()
{
  //stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  delay(500);

  servo_3.write(135); //servo looks left
  delay(500);
  leftDistance = readPing(); //find the distance on the left
  delay(1000);
  servo_3.write(45); //servo looks right
  delay(800);
  rightDistance = readPing(); //find the distance on the right
  delay(1000);
  servo_3.write(90); //servo looks straight
  delay(500);

  if (leftDistance > rightDistance) //if left is better
  {
    turnleft();
  }
  else if (rightDistance > leftDistance) //if right is better
  {
    turnright();
  }
  else //if they both suck
  {
    uturn();
  }
}
//------------------------------------------------------------
void straight() {
  // going straight
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 85);
  for (speeds = 1; speeds <= 90; speeds += 2) {
    analogWrite(enA, speeds);
    delay(5);
  }
}
//------------------------------------------------------------
void turnright()
{

  //change speeds
  analogWrite(enA, 90);
  analogWrite(enB, 90);

  //go back
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); //motor 1 back
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); //back

  delay(600);

  //stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  delay(500);
 
  //change speeds
  analogWrite(enA, 90);
  analogWrite(enB, 90);
  //turn right
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); //motor 1 back
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); //motor 2 forward

  delay(1000);

  straight();
}
//-----------------------------------------------------------------
void turnleft()
{

  //change speeds
  analogWrite(enA, 90);
  analogWrite(enB, 90);

  //go back
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); //motor 1 back
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); //back

  delay(400);

  //stop
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);

  delay(500);

  analogWrite(enA, 90);
  analogWrite(enB, 90);
  //turn left
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); //motor 1 forward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); //motor 2 back

  delay(900);

  straight();
}
//-------------------------------------------------------------------------
void uturn()
{
  //change speeds
  analogWrite(enA, 90);
  analogWrite(enB, 90);

  //go back
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH); //motor 1 back
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); //back

  delay(400);

  //slight left turn
  analogWrite(enA, 80);
  analogWrite(enB, 75);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW); //motor 1 forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); //motor 2 forward
  delay(200);

  straight();
}
//----------------------------------------------------------------------------------
int readPing() {
  delay(70);
  unsigned int uS = sonar.ping();
  int cm = uS / US_ROUNDTRIP_CM;
  return cm;
}



