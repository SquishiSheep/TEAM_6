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
ESP32SharpIR straight(ESP32SharpIR::GP2Y0A21YK0F, 29);

float testdist = 10;

void setup () {
    left.setFilterRate(0.1f);
    right.setFilterRate(0.1f);
    straight.setFilterRate(0.1f);
    //float number
    
}


void loop() {
    Serial.printLn(left.getDistanceFloat());
    Serial.printLn(right.getDistanceFloat());
    Serial.printLn(straight.getDistanceFloat());

    float left_dist = left.getDistanceFloat();
    float right_dist = right.getDistanceFloat();
    float straight_dist = straight.getDistanceFloat();

//straight path

    if(straight_dist > testdist){
        straightAhead();
    }

// right path clear
    else if (left_dist <= testdist && right_dist > testdist && straight_dist <= testdist){
        turnRight();
    }
// left only open
    else if (left_dist > testdist && right_dist <= testdist && straight_dist <= testdist){
        turnLeft();  
    }

// left and right open
    else if (left_dist > testdist && right_dist > testdist && straight_dist <= testdist){
        turnLeft(); //default behaivor is turn left
    }

//dead end
if (left == LOW && straight == LOW && right == LOW){
    flip() == TRUE;
    delay(1000);
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

void stop() {
  ServoRight.write(1500);
  ServoLeft.write(1500);
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