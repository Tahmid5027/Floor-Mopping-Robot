#include <Servo.h>
Servo servo;
const int servoPin=10;

//Sound module
int RightPin=22, LeftPin=24, ForwardPin=26, MainSensor=28;

//SONAR
int trig= A1, echo=A2;
float duration, distance; 

//motor driver
int enA=2, enB=3, in1=30, in2=31, in3=32, in4=33;

//mopper and pump
int EnA=5, EnB=6, In1=38, In2=39, In3=40, In4=41;



void setup() {
  // put your setup code here, to run once:

  //for sound sensors
  pinMode(MainSensor,INPUT);
  pinMode(RightPin,INPUT);
  pinMode(LeftPin,INPUT);
  pinMode(ForwardPin,INPUT);

  //for SONAR
  pinMode(echo,INPUT);
  pinMode(trig,OUTPUT);
  //pinMode(soundPin,INPUT);

  //for wheels
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  //for mopper and pump
  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(EnB, OUTPUT);
  
  // Initialize the servo motor to a starting position
  servo.attach(servoPin);
  servo.write(90); // 90 degrees is centered position
  delay(1000);     // Wait for the servo to stabilize
  
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  // put your main code here, to run repeatedly:
  int mainSensor= digitalRead(MainSensor);
  if(mainSensor==1){
    stopMotors();
    mopper_and_pump_ON();
    delay(3000);
    mopper_and_pump_OFF();
    delay(1000);
  }
  else{
    //sound value storing 
    int right = digitalRead(RightPin);
    int left = digitalRead(LeftPin);
    int forward= digitalRead(ForwardPin);

    //action
    if(forward==1 && right==1 && left==1){
      moveForward();
      delay(500);
      avoidObstacle();
      delay(300);
    }
    else if(forward==1 && right==0 && left==0){
      moveForward();
      delay(500);
      avoidObstacle();
      delay(300);
    }
    else if(forward==1 && right==1 && left==0){
      turnRight();
      delay(200);
      //avoidObstacle();
      //delay(3000);
    }
    else if(forward==1 && right==0 && left==1){
      turnLeft();
      delay(200);
      //avoidObstacle();
      //delay(3000);
    }
    else if(forward==0 && right==1 && left==0){
      turnRight();
      delay(500);
      //avoidObstacle();
    }
    else if(forward==0 && right==0 && left==1){
      turnLeft();
      delay(500);
    }
    else{
      avoidObstacle();
      delay(2000);
    }
  }
  delay(500);
}



void turnLeft(){
  digitalWrite(enA,HIGH);
  digitalWrite(enB,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void turnRight(){
  digitalWrite(enA,HIGH);
  digitalWrite(enB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

void stopMotors(){
  digitalWrite(enA,LOW);
  digitalWrite(enB,LOW);
}

void moveBackward(){
  digitalWrite(enA,HIGH);
  digitalWrite(enB,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);
}

void moveForward(){
  digitalWrite(enA,HIGH);
  digitalWrite(enB,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
}

int distanceMeasure(){
  digitalWrite(trig,LOW); /*to make sure that the trig pin is 0V in the first place*/
  delay(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  duration=pulseIn(echo,HIGH);
  distance=0.034*duration/2;
  return distance;
}

void avoidObstacle() {
  // Measure distance in the forward direction
  stopMotors();
  delay(1000);
  servo.write(90); // setting the default position in forward position
  delay(500);
  float forwarddistance = distanceMeasure();
  
  // Move the servo to the left position
  int i;
  for(i=90;i>=0;i--){
    servo.write(i);
    delay(10);
  }
  
  // Measure distance in the left direction
  float leftdistance = distanceMeasure();
  
  // Move the servo to the right position
  for(i=0;i<=180;i++){
    servo.write(i);
    delay(10);
  }
  
  // Measure distance in the right direction
  float rightdistance = distanceMeasure();
  
  // Reset the servo to the forward position
  servo.write(90);
  delay(500);  // Give servo time to move
  
  // Print the distance values
  Serial.print("Forward Distance: ");
  Serial.println(forwarddistance);
  Serial.print("Left Distance: ");
  Serial.println(leftdistance);
  Serial.print("Right Distance: ");
  Serial.println(rightdistance);
  
  // Make decisions based on distance measurements
  if (forwarddistance < 20) {
    // Obstacle detected in front, take avoiding action
    if (leftdistance > rightdistance) {
      turnRight();
      delay(550);
    } else {
      turnLeft();
      delay(550);
    }
  } else {
    moveForward();
    delay(850);
  }
  
  delay(30);  // Delay before the next iteration
}

void mopper_and_pump_ON(){
  /*for water pump*/
  //analogWrite(EnA,100); 
  digitalWrite(EnA,HIGH);
  digitalWrite(In1,HIGH); 
  digitalWrite(In2,LOW);
  
  /*for mopper*/
  digitalWrite(EnB,HIGH); /*for operating the water pump slowly */
  digitalWrite(In3,HIGH);
  digitalWrite(In4,LOW);
}

void mopper_and_pump_OFF(){
  digitalWrite(EnA,LOW);
  digitalWrite(EnB,LOW);
}
