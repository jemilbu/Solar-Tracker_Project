/* SolarTracker.ino
 * Authors: Jacob Milburn and Nathan Lauritsen
 * April 4, 2022
 */

//  Headers to include
#include <LiquidCrystal.h>
#include <Servo.h>
#include "EncoderMonitor.h"
#include "ClockBasics.h"
#include "ButtonDebounce.h"

//  Set up Global Variables

// Defines for LCD
LiquidCrystal LcdDriver(A5, A4, 5, 6, 7, 8);

//  Set up Program
void setup()
{
    EncoderInitialize();
    ButtonInitialize();
}

//  Main Loop
void loop()
{
}


/* Code from Lecture: 



Servo myservo;
int pos = 90;
int sens1 = 0;
int sens2 = 1;
int error = 5;
int servopin = 10;
// LCD Display
LiquidCrystal lcd(A5, A4, 5, 6, 7, 8);
// Software Timer for control 2 Hz
unsigned long ControlTimer;
#define CONTROL_INTERVAL 500
// put your setup code here, to run once:
void setup()
{
    myservo.attach(servopin);
    pinMode(sens1, INPUT);
    pinMode(sens2, INPUT);
    pinMode(servopin, OUTPUT);
    myservo.write(pos);
    delay(2000);
    // LCD display setup
    lcd.begin(16, 2);
    lcd.clear();
    // Software timer setup.
    ControlTimer = millis();
} // end of setup.
// Parameters of search algorithm
#define LightOff 500
#define LightTolerance 25
void loop()
{
    // put your main code here, to run repeatedly:
    int Total, MidPoint;
    if (millis() - ControlTimer >= CONTROL_INTERVAL)
    {
        // Read in light levels.
        Total = analogRead(sens1);
        MidPoint = analogRead(sens2);
        // Send results to display.
        lcd.clear();
        lcd.print("Tot. ");
        lcd.print(Total);
        lcd.print("->");
        lcd.print(5.0 * (float)Total / 1024.0, 3);
        lcd.setCursor(0, 1);
        lcd.print("Bal. ");
        lcd.print(MidPoint);
        lcd.print("->");
        lcd.print(5.0 * (float)MidPoint / 1024.0, 3);
        // Update timer.
        ControlTimer += CONTROL_INTERVAL;
        // check if motion is needed.
        if (Total < LightOff)
            if (abs(Total - MidPoint) > LightTolerance)
            {
                if (Total > 2 * MidPoint)
                    pos += 3;
                else
                    pos -= 3;
                myservo.write(pos);
            } // end of position update.
    }         // end of timer if
} // end of loop.

*/

/* Clock from lecture:
Appendix F: Main Arduino Code for Morse Code Transmit
#include <LiquidCrystal.h>
// Defines for LCD
LiquidCrystal LcdDriver(A5, A4, 5, 6, 7, 8);
#include "ASCIIToMorseCode.h"
#include "TriColorLed.h"
#include "ClockBasics.h"
#include "ButtonDebounce.h"
#include "EncoderMonitor.h"
int EncoderTracking = 0;
// Timer to control Clock, TxMorseCode, Led and Encoder.
#define CLOCK_TIME 1000
#define DOT_TIME 50
#define LED_FLASH_TIME 1000
#define ENCODER_INTERVAL 250
unsigned long ClkTimer, TxTimer, LedTimer, EncoderTimer;
unsigned long INTERVAL = 1000;
// setup code, ran once:
void setup()
{
    SetUpLED();
    pinMode(MorseCodePin, OUTPUT); // Set to control beeper
    // Set up clock
    // first the inputs for setting the clock.
    ButtonInitialize(4); // connect to push button
    EncoderInitialize();   // Monitor encoder.
    EncoderTracking = EncoderValue;
    // Set initial time
    CLK_Hours = 9;
    CLK_Minutes = 58;
    CLK_Seconds = 58;
    // Lcd and Serial
    LcdDriver.begin(16, 2);
    Serial.begin(9600);    // Serial Port on
    TxTimer = millis();    // Timer for Morse Code transmitting.
    LedTimer = millis();   // Timer for led flash
}
int Lcd_Update = 0;
// main code, ran repeatedly:
void loop()
{
    // Clock Control
    if (millis() - ClkTimer >= CLOCK_TIME)
    {
        if (clockState == CLOCK_RUNNING)
            UpdateClock();
        Lcd_Update = 1; // Set flag to update the display
        ClkTimer += CLOCK_TIME;
        LcdDriver.clear();
        SendClock();  // Place time
        // Set the cursor to indicate what is being set.
        switch (clockState)
        {
        case CLOCK_SET_HOURS:
            LcdDriver.setCursor(0, 0);
            break;
        case CLOCK_SET_MINUTES:
            LcdDriver.setCursor(3, 0);
            break;
        case CLOCK_SET_SECONDS:
            LcdDriver.setCursor(6, 0);
            break;
        case CLOCK_RUNNING:
            LcdDriver.setCursor(0, 1);
        }
        LcdDriver.blink();
        Lcd_Update = 0; // Clear lcd update flag.
    }
    // Flash LED
    if (millis() - LedTimer >= INTERVAL)
    {
        SwitchLED();
        // Update timer
        LedTimer += INTERVAL;
        // Change interval after each cycle.
        if (LedState == RedLedPin)
        {
            INTERVAL -= 50;
            if (INTERVAL < 100)
                INTERVAL = 500;
        } // end of update of interval change
    } // end of led timer.
    // Timer to control time of beeper on and off.
    if (millis() - TxTimer >= DOT_TIME)
    {
        // Based on state,
        if (TxNextState())
            digitalWrite(MorseCodePin, HIGH); // Beeper on.
        else
            digitalWrite(MorseCodePin, LOW); // beeper off.
        TxTimer += DOT_TIME; // Update timer
    }
    // Watch button and encoder to set clock
    if (ButtonNextState( digitalRead(4)) == 1) // Button pressed,
        SettingClock(1, 0);  // So update state.
    else if ((EncoderValue - EncoderTracking) >= 4)
    { // The encoder has moved by one detent
        SettingClock(0, -1);  // Indicate positive travel
        LcdDriver.clear();
        SendClock();  // Place time
        EncoderTracking += 4;   // and move tracking variable.
    }
    else if ((EncoderValue - EncoderTracking) <= -4)
    { // The encoder has moved by one detent in the opposite direction
        SettingClock(0, +1);  // Indicate negative travel
        LcdDriver.clear();
        SendClock();  // Place time
        EncoderTracking -= 4;   // and Update Tracking accordingly
    }
}// End of loop.

*/