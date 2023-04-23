#include <AccelStepper.h>
#include <Stepper.h> 
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  // put your setup code here, to run once:
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(30);
  stepper.setCurrentPosition(0); 
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
  delay(3000); 
}

// Move back and forth 
void loop() {
  // put your main code here, to run repeatedly:
  stepper.moveTo(50); 
  stepper.runToPosition(); 
  delay(1000); 
  stepper.moveTo(0); 
  stepper.runToPosition(); 
//  stepper.move(0); 
//  stepper.run();
//  digitalWrite(stepPin,HIGH); 
}
