#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

ESP32SharpIR left(ESP32SharpIR::GP2Y0A21YK0F, 27);
ESP32SharpIR right(ESP32SharpIR::GP2Y0A21YK0F, 28);
ESP32SharpIR forward(ESP32SharpIR::GP2Y0A21YK0F, 29);

void setup () {
    left.setFilterRate(0.1f);
    right.setFilterRate(0.1f);
    straight.setFilterRate(0.1f);
    //float number

    straightAhead() == FALSE;
    turnLeft() == FALSE;
    turnRight() == FALSE;
    reset() == TRUE;
    
}

void loop() {
    Serial.printLn(left.getDistanceFloat());
    Serial.printLn(right.getDistanceFloat());
    Serial.printLn(straight.getDistanceFloat());

//straight path
  if (left == LOW && straight == HIGH && right == LOW){
   straightOpen() == TRUE;
  }

//right only open
  if (left == LOW && straight == LOW && right == HIGH){
    turnRight() == TRUE;
    delay(1000);
    straightAhead() == TRUE;
  }

//left only open
  if (left == HIGH && straight == LOW && right == LOW){
    turnLeft() == TRUE;
    delay(1000);
    straightAhead() == TRUE;
  }
  
//dead end
if (left == LOW && straight == LOW && right == LOW){
    flip() == TRUE;
    delay(1000);
    straightAhead() == TRUE;
  }

  //if left and right open, choose left
  //haha ha
  //jk i need to change
  //these to LOW AND HIGH
  //LOL
  if (turnLeft() == TRUE && turnRight() == TRUE){
    turnRight() == FALSE;
  }

 // if left and straight open, choose left
 if (turnLeft() == TRUE && straightOpen() == TRUE){
  straightOpen() == FALSE;
 }

 //if left and straight and right open, choose left
 if (turnLeft() == TRUE && straightOpen() == TRUE && turnRight() == TRUE){
  straightOpen() == FALSE;
  turnRight() == FALSE;
 }

  //if straight and right open, go straight
 if (straightOpen() == TRUE && turnRight() == TRUE){
  turnRight() == FALSE;
 }

  delay(1000);

  reset();
}

void goStraight(){
  straightAhead() == TRUE;
}

void turnLeft() {
  //change values based on testing
  ServoRight.write(2000);
  servoLeft.write(1501);
}

void turnRight()  {
  ServoRight.write(1501);
  ServoLeft.write(2000);
}

void straightAhead()  {
  ServoRight.write(2000);
  ServoLeft.write(2000);
}

void flip() {
  ServoLeft.write(1000);
  ServoRight.write(2000);
}

void reset(){
  turnLeft() == FALSE;
  turnRight() == FALSE;
  straightAhead() == FALSE;
  flip() == FALSE;
  goStraight() == FALSE;
}

//helper functions
//seperate components
//initialize functions
//void sampleDistance(int one, int two){
  //  int three = one + two
//}

//void loop (){
 //   sampleDistance();
//}


//call functions from different files
//if calling another file, make a header file for it than an include
//in header file put the void sampleDistance (){}