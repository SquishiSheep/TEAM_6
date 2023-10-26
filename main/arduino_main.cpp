//****************************************************************************
//Copyright 2021 Ricardo Quesada

//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at

    //http://www.apache.org/licenses/LICENSE-2.0

//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.
//****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

//color sensor headers
#include <Wire.h>
#include <Arduino_APDS9960.h>
#include <bits/stdc++.h>

//color sensor definitions
#define APDS9960_INT 2
#define I2C_SDA 21
#define I2CSCL 22
#define I2C_FREQ 100000

//flash for comp
#define ONBOARD_LED 3

//Color sensor unit & I2C unit
TwoWire I2C_0 = TwoWire(0);
APSD9960 apds = APDS9960(I2C_0; APDS9960_INT);

void setup(){
    //sets up I2C protocol
    I2C_0.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    // Set up color sensor
    apds.setInterruptPin(APDS9960_INT);
    apds.begin();
    Serial.begin(115200);

    //LED
    pinMode(ONBOARD_LED,OUTPUT);
    }

void loop () {
    int r, g, b, a;

    //wait until color is ready from sensor
    while (!apds.colorAvailable()){
        delay(5);
    }

    //int to set color to find
    int findThisColor = -1

    //read color from sensor
    apds.readColor(r, g, b, a);

    //print color in decimal
    Serial.print("RED: ");
    Serial.printIn(r);
    Serial.print("GREEN: ");
    serial.printIn(g);
    Serial.print("BLUE: ");
    Serial.printIn(b);
    Serial.print("AMBIENT: ");
    Serial.printIn(a);

//flashing is 5 seconds in between reading a color again

//also change r,g,b percentages based on testing of color sensor

//change last delay of each if based on how large the color sensor field is
//and how much the robot has to move

    if (findThisColor == -1){
        findThisColor = apds.readColor();

    } else if (apds.readColor == (r >= 0.60)){
        delay(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100)
        digitalWrite(ONBOARD_LED,LOW);
        delay(3900);
        servoLeft.write(2000);
        servoRight.write(2000);
        delay(2000);

    } else if (apds.readColor == (g >= 0.60){
        delay(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(100);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(3800);
        servoLeft.write(2000);
        servoRight.write(2000);
        delay(2000);

    }  else if (apds.readColor == (b >= 0.60){
        delay(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(100);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(100);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(3600);
        servoLeft.write(2000);
        servoRight.write(2000);
        delay(2000);
    
    //yippee we found the color
    } else if (apds.readColor == findThisColor){
        servoRight.write(2000);
        servoLeft.write(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(100);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(100);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digtialWrite(ONBOARD_LED,LOW);
        delay(500);
        //hopefully this makes a full 360 and flashes
    }
}