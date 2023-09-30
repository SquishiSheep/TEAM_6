#include "sdkconfig.h"
#include <Arduino.h>

//Checkpoint 3 - Blink the onboard LED
//We know pin 2 on the board is the built-in LED, but we need to tell the code that.
#define LED 2 // Associate the variable LED with the number 2 
//int LED = 2;  // this way works too (useful for if we want to change what pin LED corresponds to later on in the program

// Arduino setup function. Runs in CPU 1
//put the code you want to run once in setup()
void setup() { 

//Set the pin mode of out LED pin 
//Template: pinMode (pin number, mode); 
//INPUT, OUTPUT, PULLUP, etc, are all differnt modes we can use

pinMode(LED, OUTPUT);

//We want LED pin to be an output, since we are sending an on or off to that pin 

}

// Arduino loop function. Runs in CPU 1
//Put the code you want to run an infinite loop here
void loop() {

    // the digitalWrite(pin, value); command allows us to send boolean data (1 or 0) to an output pin

    digitalWrite(LED, HIGH); // Set the LED pin HIGH to turn it on
    delay(500);              // Half-second delay (500ms)
    digitalWrite(LED, LOW);  // Set the LED pin LOW to turn it off
    delay(500);              // Half-second delay (500ms)


    //If we didnt have the delay(500); commands, the LED would blink so fast that we wouldn't be able to see it blink
}
