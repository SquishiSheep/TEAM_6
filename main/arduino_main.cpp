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

//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using "Serial" you can use Bluepad32 "Console" class instead.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

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
#define APDS9960_INT 5
#define I2C_SDA 20
#define I2C_SCL 21
#define I2C_FREQ 100000

//flash for comp
#define ONBOARD_LED 2

Servo servoLeft;
Servo servoRight;
Servo servoDump;

//autonomous task booleans
bool doingLineSensorTask = false;
bool doingColorSensorTask = false;
bool doingMazeFollowTask = false;
bool doingPizzaDeliveryTask = false;

//Color sensor unit & I2C unit
TwoWire I2C_0 = TwoWire(0);
APDS9960 apds = APDS9960(I2C_0, APDS9960_INT);

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

QTRSensors qtr;
int16_t position = 3500;

ESP32SharpIR left(ESP32SharpIR::GP2Y0A21YK0F, 25);
ESP32SharpIR right(ESP32SharpIR::GP2Y0A21YK0F, 26);
ESP32SharpIR straight(ESP32SharpIR::GP2Y0A21YK0F, 27);

//change pin numebr to robot

//int servoRight = 23;
//int servoLeft = 24;
//WHY DID I ADD THIS


//change # based on sensor
float testdist = 10;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == nullptr) {
            // Console.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            // GamepadProperties properties = gp->getProperties();
            // Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName(), properties.vendor_id,
            //                properties.product_id);
            myGamepads[i] = gp;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        // Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
    }
}



void onDisconnectedGamepad(GamepadPtr gp) {
    bool foundGamepad = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myGamepads[i] == gp) {
            // Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad) {
        // Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
    }
}

void lineSensor(){
//if blank pressed if while 
 uint16_t sensors[8];
    position = abs(7000-qtr.readLineBlack(sensors));

    Serial.print(" 1: ");
    Serial.print(sensors[0]);
    Serial.print(" 2: ");
    Serial.print(sensors[1]);
    Serial.print(" 3: ");
    Serial.print(sensors[2]);
    Serial.print(" 4: ");
    Serial.print(sensors[3]);
    Serial.print(" 5: ");
    Serial.print(sensors[4]);
    Serial.print(" 6: ");
    Serial.print(sensors[5]);
    Serial.print(" 7: ");
    Serial.print(sensors[6]);
    Serial.print(" 8: ");
    Serial.print(sensors[7]);
    Serial.print(" Pos: ");
    Serial.println(position);

//3200 <= center position <= 4000
if(position > 3600){ //if to right of line
//1000 (max negative speed) 1500 (0 speed) 2000 (max positive speed)

    servoRight.write(1600);
    servoLeft.write(1400);
}


else if(position < 3400){ // if to left of line
//1000 1500 2000

    servoRight.write(1400);
    servoLeft.write(1600);
}
else{
    servoRight.write(2000);
    servoLeft.write(2000);
}
    delay(10);
}
void turnLeft() {
  //change values based on testing
  servoRight.write(2000);
  servoLeft.write(1500);

}

void turnRight()  {
  servoRight.write(1500);
  servoLeft.write(2000);
}

void straightAhead()  {
  servoRight.write(2000);
  servoLeft.write(2000);
}

void stop() {
  servoRight.write(1500);
  servoLeft.write(1500);
}

void flip() {
  servoLeft.write(1000);
  servoRight.write(2000);
}

void colorSensor(){
 int r, g, b, a;

    //wait until color is ready from sensor
    while (!apds.colorAvailable()){
        delay(5);
    }

    //int to set color to find
    int findThisColor = -1;

    //read color from sensor
    apds.readColor(r, g, b, a);

    //print color in decimal
    Serial.print("RED: ");
    Serial.print(r);
    Serial.print(" GREEN: ");
    Serial.print(g);
    Serial.print(" BLUE: ");
    Serial.print(b);
    Serial.print(" AMBIENT: ");
    Serial.println(a);

    const int colorThreshold = 200;
//flashing is 5 seconds in between reading a color again

//also change r,g,b percentages based on testing of color sensor

//change last delay of each if based on how large the color sensor field is
//and how much the robot has to move

    if (findThisColor == -1){
        findThisColor = apds.readColor(r, g, b, a);


    } else if (r > colorThreshold && g < colorThreshold && b < colorThreshold){
        delay(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(100);
        digitalWrite(ONBOARD_LED,LOW);
        delay(3900);
        servoLeft.write(2000);
        servoRight.write(2000);
        delay(2000);

    } else if (g > colorThreshold && r < colorThreshold && b < colorThreshold) {
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

    }  else if (b > colorThreshold && r < colorThreshold && g < colorThreshold) {
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
    } else if (findThisColor) {
        servoRight.write(2000);
        servoLeft.write(1000);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(450);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(450);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        digitalWrite(ONBOARD_LED,HIGH);
        delay(450);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
        //hopefully this makes a full 360 and flashes

    }
}

void mazeFollow(){


    Serial.println(left.getDistanceFloat());
    Serial.println(right.getDistanceFloat());
    Serial.println(straight.getDistanceFloat());

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
    else if (left_dist <= testdist && right_dist <= testdist && straight_dist <= testdist){
        flip();
    }
// if left and right open, choose left
    else if (left_dist > testdist && right_dist > testdist && straight_dist <= testdist){
        turnLeft();
    }
    //if left and straight open
     else if (left_dist > testdist && right_dist <= testdist && straight_dist > testdist){
        turnLeft();
    }
    //if straight and right open, go straight
     else if (left_dist <= testdist && right_dist > testdist && straight_dist > testdist){
        turnLeft();
    }
    //if all open
      else if (left_dist > testdist && right_dist > testdist && straight_dist > testdist){
        turnLeft();
    }
}



//
//
//
//
//
// Arduino setup function. Runs in CPU 1
void setup() {
    // Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    
     pinMode(ONBOARD_LED,OUTPUT);

    // Setup the Bluepad32 callbacks
    left.setFilterRate(0.1f);
    right.setFilterRate(0.1f);
    straight.setFilterRate(0.1f);

    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    servoLeft.setPeriodHertz(50);
    servoRight.setPeriodHertz(50);
    servoLeft.attach(23, 1000, 2000);
    servoRight.attach(5, 1000, 2000);

    //sets up I2C protocol
    I2C_0.begin(I2C_SDA, I2C_SCL, I2C_FREQ);

    // Set up color sensor
    apds.setInterruptPin(APDS9960_INT);
    apds.begin();

      qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){36, 39, 34, 35,32,33,25,26}, 8);

    Serial.begin(115200);

    for (uint8_t i = 0; i < 70; i++)
    {
        Serial.println("calibrating");
        qtr.calibrate();
            //Crappy Auto-calibration:
         /*   servoRight.write(2000);
            servoLeft.write(1000);

            delay(300);
            servoLeft.write(2000);
            servoRight.write(1000);
            delay(300);
            */
           delay(20);

    }    
        digitalWrite(ONBOARD_LED,HIGH);
        delay(1000);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
                digitalWrite(ONBOARD_LED,HIGH);
        delay(1000);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);
                digitalWrite(ONBOARD_LED,HIGH);
        delay(1000);
        digitalWrite(ONBOARD_LED,LOW);
        delay(500);

 }


// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the gamepad info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The gamepads pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected()) {

            if(!doingLineSensorTask && !doingColorSensorTask && !doingMazeFollowTask && !doingPizzaDeliveryTask){
                if(myGamepad->a()){
                    Serial.println("---A pressed---");
                    doingLineSensorTask = true;
                }
                else if(myGamepad->b()){
                    Serial.println("---B pressed---");
                   // doingColorSensorTask = true;
                }
                else if(myGamepad->y()){
                    Serial.println("---Y pressed---");
                    doingMazeFollowTask = true;
                }
                else if(myGamepad->x()){
                    Serial.println("---X pressed---");
                    doingPizzaDeliveryTask = true;
                }
                else{ 

                    doingLineSensorTask = false;
                    doingColorSensorTask = false;
                    doingMazeFollowTask = false;
                    doingPizzaDeliveryTask = false;
                    //do no autonomous task, allow for manual control
                }
                
            }
            if (myGamepad->thumbL())
            { //if back/select button pressed, stop autonomous task, allow for manual control
                doingLineSensorTask = false;
                doingColorSensorTask = false;
                doingMazeFollowTask = false;
                doingPizzaDeliveryTask = false;
                Serial.println("---thumbL pressed---");
            }
            

            if (doingLineSensorTask && !doingColorSensorTask && !doingMazeFollowTask && !doingPizzaDeliveryTask){//line sensor task run in the loop 
                Serial.println("Line Time!!");
                lineSensor();
            }

            else if (!doingLineSensorTask && doingColorSensorTask && !doingMazeFollowTask && !doingPizzaDeliveryTask){//color sensor task
                Serial.println("Color Time!!");

                colorSensor();
            }

            else if (!doingLineSensorTask && !doingColorSensorTask && doingMazeFollowTask && !doingPizzaDeliveryTask){ //maze follow task
                Serial.println("Maze Time!!");
                mazeFollow();
            }

            else if (!doingLineSensorTask && !doingColorSensorTask && !doingMazeFollowTask && doingPizzaDeliveryTask){ //pizza delivery task
                Serial.println("Pizza Time!!");
                delay(100);
                //pizzaDelivery();
            }

            else{ //manual control
                Serial.println("Manual Control!!");
                servoLeft.write( ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );
                servoRight.write( ((((float) myGamepad->axisRY()) / 512.0f) * 500) + 1500 );
            }


            
            



            // Another way to query the buttons, is by calling buttons(), or
            // // miscButtons() which return a bitmask.
            // // Some gamepads also have DPAD, axis and more.
            // Console.printf(
            //     "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
            //     "%4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
            //     i,                        // Gamepad Index
            //     myGamepad->dpad(),        // DPAD
            //     myGamepad->buttons(),     // bitmask of pressed buttons
            //     myGamepad->axisX(),       // (-511 - 512) left X Axis
            //    myGamepad->axisY(),       // (-511 - 512) left Y axis
            //     myGamepad->axisRX(),      // (-511 - 512) right X axis
            //     myGamepad->axisRY(),      // (-511 - 512) right Y axis
            //     myGamepad->brake(),       // (0 - 1023): brake button
            //     myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
            //     myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
            // );

            // You can query the axis and other properties as well. See Gamepad.h
            // For all the available functions.
        }

    vTaskDelay(1);
    delay(100);

    }

  }




    
