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

//change pin numebr to robot
int ServoRight = 1;
int ServoLeft = 2;
//change # based on sensor
float testdist = 10;

void setup () {
    left.setFilterRate(0.1f);
    right.setFilterRate(0.1f);
    straight.setFilterRate(0.1f);
    //float number
    ServoLeft.attach(1);
    ServoRight.attach(2);
    
}


void loop() {
    Serial.printIn(left.getDistanceFloat());
    Serial.printIn(right.getDistanceFloat());
    Serial.printIn(straight.getDistanceFloat());

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

//  dead end
    else if (left_dist <= testdist && right_dist <= testdist && straight_dish <= testdist){
        flip();
    }
// if left and right open, choose left
    else if (left_dist > testdist && right_dist > testdist && straight_dish <= testdist){
        turnLeft();
    }
    //if left and straight open
     else if (left_dist > testdist && right_dist <= testdist && straight_dish > testdist){
        turnLeft();
    }
    //if straight and right open, go straight
     else if (left_dist <= testdist && right_dist > testdist && straight_dish > testdist){
        turnLeft();
    }
    //if all open
      else if (left_dist > testdist && right_dist > testdist && straight_dish > testdist){
        turnLeft();
    }
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