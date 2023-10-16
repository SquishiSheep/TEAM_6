//
// Copyright 2021 Ricardo Quesada

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//  http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//***************************************************************************
//

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

//
// README FIRST, README FIRST, README FIRST
// a
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

// GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
// void onConnectedGamepad(GamepadPtr gp) {
//     bool foundEmptySlot = false;
//     for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
//         if (myGamepads[i] == nullptr) {
//             // Console.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
//             // Additionally, you can get certain gamepad properties like:
//             // Model, VID, PID, BTAddr, flags, etc.
//             // GamepadProperties properties = gp->getProperties();
//             // Console.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName(), properties.vendor_id,
//             //                properties.product_id);
//             myGamepads[i] = gp;
//             foundEmptySlot = true;
//             break;
//         }
//     }
//     if (!foundEmptySlot) {
//         // Console.println("CALLBACK: Gamepad connected, but could not found empty slot");
//     }
// }

// void onDisconnectedGamepad(GamepadPtr gp) {
//     bool foundGamepad = false;

//     for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
//         if (myGamepads[i] == gp) {
//             // Console.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
//             myGamepads[i] = nullptr;
//             foundGamepad = true;
//             break;
//         }
//     }

//     if (!foundGamepad) {
//         // Console.println("CALLBACK: Gamepad disconnected, but not found in myGamepads");
//     }
// }

Servo servoLeft;
Servo servoRight;

// ESP32SharpIR sensor1( ESP32SharpIR::GP2Y0A21YK0F, 27);
QTRSensors qtr;

// Arduino setup function. Runs in CPU 1
void setup()
{
    // // BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    // BP32.forgetBluetoothKeys();

    servoLeft.setPeriodHertz(50);
    servoRight.setPeriodHertz(50);
    servoLeft.attach(13, 1000, 2000);
    servoRight.attach(14, 1000, 2000);
/*
green:36:1 
white:39:2
orange:34:3
grey:35:4
brown:32:5
yellow:33:6
purple:25:7
blue:26:8
*/
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){36, 39, 34, 35,32,33,25,26}, 8);
    // and change 3 to 8
    // qtr.setEmitterPin(2);

    Serial.begin(115200);

    for (uint8_t i = 0; i < 70; i++)
    {
        Serial.println("calibrating");
        qtr.calibrate();
         /*   servoRight.write(2000);
            servoLeft.write(1000);

            delay(300);
            servoLeft.write(2000);
            servoRight.write(1000);
            delay(300);
            */
           delay(20);
    }
    servoLeft.write(1500);
    servoRight.write(1500);
/*
    // Serial.begin(115200);
    // sensor1.setFilterRate(0.1f);

    // Serial.begin(115200);
    // sensor1.setFilterRate(0.1f);

    // qtr.setTypeRC(); // or setTypeAnalog()
    // qtr.setSensorPins((const uint8_t[]) {12,13,14}, 3);
    // for (uint8_t i = 0; i < 250; i++)
    // {
    //     Serial.println("calibrating");
    //     qtr.calibrate();
    //     delay(20);
    // }
    // qtr.calibrate();
    */
}
int16_t position = 3500;
// Arduino loop function. Runs in CPU 1


    

void loop()
{
    /*
    // BP32.update();

    // // It is safe to always do this before using the gamepad API.
    // // This guarantees that the gamepad is valid and connected.
    // for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    //     GamepadPtr myGamepad = myGamepads[i];

    //     if (myGamepad && myGamepad->isConnected()) {

    //         servo.write( ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );

    //         // Another way to query the buttons, is by calling buttons(), or
    //         // miscButtons() which return a bitmask.
    //         // Some gamepads also have DPAD, axis and more.
    //         // Console.printf(
    //         //     "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
    //         //     "%4d, brake: %4d, throttle: %4d, misc: 0x%02x\n",
    //         //     i,                        // Gamepad Index
    //         //     myGamepad->dpad(),        // DPAD
    //         //     myGamepad->buttons(),     // bitmask of pressed buttons
    //         //     myGamepad->axisX(),       // (-511 - 512) left X Axis
    //         //     myGamepad->axisY(),       // (-511 - 512) left Y axis
    //         //     myGamepad->axisRX(),      // (-511 - 512) right X axis
    //         //     myGamepad->axisRY(),      // (-511 - 512) right Y axis
    //         //     myGamepad->brake(),       // (0 - 1023): brake button
    //         //     myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
    //         //     myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
    //         // );

    //         // You can query the axis and other properties as well. See Gamepad.h
    //         // For all the available functions.
    //     }
    // }

    // Serial.println(sensor1.getDistanceFloat());
*/
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

