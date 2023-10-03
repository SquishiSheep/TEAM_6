#include "sdkconfig.h"
#include <Arduino.h>
#include <Bluepad32.h>

#include <ESP32Servo.h>
#include <ESP32SharpIR.h>
#include <QTRSensors.h>

//Checkpoint 4 - Servo Control 

//Diego: Workshop
Servo servo;

void setup() {
    servo.setPeriodHertz(50);
    servo.attach(13, 1000, 2000);
}

void loop () {
    servo.write(1000);
    delay(1000);
    servo.write(2000);
    delay(1000);
}
