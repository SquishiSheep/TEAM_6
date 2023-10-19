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

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

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

Servo servoLeft;
Servo servoRight;

ESP32SharpIR sensor1( ESP32SharpIR::GP2Y0A21YK0F, 27);
QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

// Arduino setup function. Runs in CPU 1
void setup() {
    // Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
    servoLeft.setPeriodHertz(50);
    servoRight.setPeriodHertz(50);
    servoLeft.attach(13, 1000, 2000);
    servoRight.attach(14, 1000, 2000);

           // create an object for three QTR-xA sensors on analog inputs 0, 2, and 6
    qtr.setTypeAnalog();
    qtr.setSensorPins((const uint8_t[]){5, 17, 6}, 3);
    //or 5,17,16,21,23,19,18, and change 3 to 8
    //qtr.setEmitterPin(2);
    
    Serial.begin(115200);

    
    // Serial.begin(115200);
    sensor1.setFilterRate(0.1f);

    // qtr.setTypeRC(); // or setTypeAnalog()
    // qtr.setSensorPins((const uint8_t[]) {12,13,14}, 3);
     for (uint8_t i = 0; i < 250; i++)
        {
        Serial.println("calibrating");
        qtr.calibrate();
        delay(20);
    }
     qtr.calibrate();
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

            servoLeft.write( ((((float) myGamepad->axisY()) / 512.0f) * 500) + 1500 );
            servoRight.write( ((((float) myGamepad->axisRY()) / 512.0f) * 500) + 1500 );

            // Another way to query the buttons, is by calling buttons(), or
            // miscButtons() which return a bitmask.
            // Some gamepads also have DPAD, axis and more.
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
    }
    // create an object for four QTR-xRC sensors on digital pins 0 and 9, and on analog
// inputs 1 and 3 (which are being used as digital inputs 15 and 17 in this case)
    qtr.read(sensorValues);

    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected()) {

        for (uint8_t i = 0; i < SensorCount; i++)
        {
            Serial.print(sensorValues[i]);
            Serial.print('\t');
        }

        Serial.println(sensor1.getDistanceFloat());

        uint16_t sensors[3];
        int16_t position = qtr.readLineBlack(sensors);
        int16_t error = position - 1000;

        if (error < 0)
        {
            Serial.println("On the left");
            servoLeft.write(950);
            servoRight.write(1000);
        }
        if (error > 0)
        {
            Serial.println("On the right");
            servoLeft.write(1000);
            servoRight.write(950);
        }
        if(error == 0){
        Serial.println("Straight Ahead");  
        servoLeft.write(1000);
        servoRight.write(1000);
        }

        }
    vTaskDelay(1);
    delay(100);
    }
}
//fidjufs
//follow the line