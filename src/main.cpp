// COMP-10184 – Mohawk College
// PIR Sensor Test Application
// Turn on the blue LED with motion is detected.
// I, Damanpreet Singh, student number 000741359, certify that all code submitted is my own work;
// that I have not copied it from any other source.
// I also certify that I have not allowed my work to be copied by others.

#include <Arduino.h>

// digital input pin definitions
#define PIN_PIR D5
#define PIN_BUTTON D6
#define BUTTON_DELAY 500
#define LIGHT_BLINK_DELAY 250

// *************************************************************
void setup()
{
  // configure the USB serial monitor
  Serial.begin(115200);

  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);

  // PIR sensor is an INPUT
  pinMode(PIN_PIR, INPUT);

  // Button is an INPUT
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
}

// *************************************************************
void loop()
{
  bool bPIR ;
  // read PIR sensor (true = Motion detected!).  As long as there
  // is motion, this signal will be true.  About 2.5 seconds after
  // motion stops, the PIR signal will become false.
  bPIR = digitalRead(PIN_PIR);
  digitalWrite(LED_BUILTIN, !bPIR);

  // boolean to keep alarm on by default
  bool DONT_DISABLE_ALARM = true;

  // send the PIR signal directly to the LED
  // but invert it because true = LED off!
  if(bPIR){
    bool ibutton;
    ibutton = digitalRead(PIN_BUTTON);
    // boolean to track if button was pressed
    bool buttonPressed = false;

    //for loop runs for 10 sec with 20 reps of 250 ms to blink the LED
    for (int i = 0; i < 20; i++)
    {
      // if button is pressed, turn off alarm and botton boolean is updated 
      // same statement is used at hte end of loop as well
      if (!digitalRead(PIN_BUTTON))
      {
        DONT_DISABLE_ALARM = false;
        buttonPressed = true;
        delay(10);
        break;
      }
      //ibutton = !(digitalRead(PIN_BUTTON));
      digitalWrite(LED_BUILTIN, HIGH);
      delay(LIGHT_BLINK_DELAY);
      digitalWrite(LED_BUILTIN, LOW);
      delay(LIGHT_BLINK_DELAY);
      if (!digitalRead(PIN_BUTTON))
      {
        DONT_DISABLE_ALARM = false;
        buttonPressed = true;
        delay(10);
        break;
      }
    }
    // delay has been added to captures the button press and update variables accordingly
    // as sometime a the other thread may try to move quickly to next loops
    delay(BUTTON_DELAY);

    // if button was not pressed, led will stay on
    if(!buttonPressed){
      while (ibutton == HIGH)
      {
        digitalWrite(LED_BUILTIN, LOW);
        delay(BUTTON_DELAY);
        ibutton = digitalRead(PIN_BUTTON);
      }
    }
    // if button was pressed before 10 sec , alarm will be turned off
    if (!DONT_DISABLE_ALARM)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      bool doloop = true;
      // Loop to keep alarm state disabled until botton is pressed again
      while (doloop)
      {
        if (!digitalRead(PIN_BUTTON)){
          doloop = false;
        }
        delay(BUTTON_DELAY);
      }
    }
  }
}
