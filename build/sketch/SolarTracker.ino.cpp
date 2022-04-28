#include <Arduino.h>
#line 1 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
#line 1 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
/* SolarTracker.ino
 * Authors: Jacob Milburn and Nathan Lauritsen
 * April 4, 2022
 */

/* Goals:
 * - Display Clock and Solar tracker data (angle) at the same time 
 * - Clean up code
 */

//  Headers to include
#include <LiquidCrystal.h>
#include <Servo.h>
#include "EncoderMonitor.h"
#include "ClockBasics.h"
#include "ButtonDebounce.h"

//  Set up Global Variables
int pos = 90;
int sens1 = 0;
int sens2 = 1;
int error = 5;
int servopin = 10;
int EncoderTracking = 0;
int Lcd_Update = 0;
bool printAngle = true;

//  Software Timer for control 2 Hz
unsigned long ControlTimer;
#define CONTROL_INTERVAL 500

// Parameters of search algorithm
#define LightOff 500
#define LightTolerance 25

// Timer to control Clock and Encoder.
#define CLOCK_TIME 1000
#define DOT_TIME 50
#define ENCODER_INTERVAL 250
unsigned long ClkTimer, TxTimer, EncoderTimer;
unsigned long INTERVAL = 1000;

//  Defines for LCD
LiquidCrystal lcd(A5, A4, 5, 6, 7, 8);

//  Defines for Servo
Servo myservo;

// Send Hours, Minutes and Seconds to a display.
#line 50 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
void SendClock();
#line 75 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
void setup();
#line 112 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
void loop();
#line 50 "c:\\Users\\Owner\\Pictures\\ECE_241\\Project\\SolarTracker\\SolarTracker.ino"
void SendClock()
{
    // Check if leading zero needs to be sent
    if (CLK_Hours < 10)
    {
        lcd.print("0");
    }
    lcd.print(CLK_Hours); // Then send hours
    lcd.print(":");       // And separator
    // Check for leading zero on Minutes.
    if (CLK_Minutes < 10)
    {
        lcd.print("0");
    }
    lcd.print(CLK_Minutes); // Then send Minutes
    lcd.print(":");         // And separator
    // Check for leading zero needed for Seconds.
    if (CLK_Seconds < 10)
    {
        lcd.print("0");
    }
    lcd.print(CLK_Seconds); // Then send Seconds
} // End of SendClock()

// Setup Program
void setup()
{
    //  Call .h file Inits
    EncoderInitialize();
    ButtonInitialize();

    //  Attach Servo
    myservo.attach(servopin);

    // Set Pin types
    pinMode(sens1, INPUT);
    pinMode(sens2, INPUT);
    pinMode(servopin, OUTPUT);

    //  Write servo to starting position and wait for motion to complete
    myservo.write(pos);
    delay(2000);

    // LCD display setup
    lcd.begin(16, 2);
    lcd.clear();

    // Software timer setup.
    ControlTimer = millis();

    EncoderTracking = encoderPosition;

    // Set initial time
    CLK_Hours = 11;
    CLK_Minutes = 59;
    CLK_Seconds = 59;

    // Lcd and Serial
    Serial.begin(9600); // Serial Port on
} // end of setup.

// Start Main loop
void loop()
{
    int Total, MidPoint;

    if (millis() - ControlTimer >= CONTROL_INTERVAL) //  If specified interval has passed
    {
        // Read in light levels.
        Total = analogRead(sens1);
        MidPoint = analogRead(sens2);
        // Send results to display.
        if (printAngle)
        {
            lcd.clear();
            SendClock(); // Place time
            lcd.setCursor(0, 1);
            lcd.print("Angle:");
            lcd.print(pos);
            lcd.print((char)223); // Degrees Symbol
        }

        // Check if there is enough light
        if (Total < LightOff)
        {
            if (abs(Total - MidPoint) > LightTolerance)
            {
                if (Total > 2 * MidPoint && pos <= 177)
                {
                    pos += 3;
                }
                else if (Total < 2 * MidPoint && pos >= 3)
                {
                    pos -= 3;
                }
                else
                {
                    pos = pos;
                }
            } // end of position update.
        }
        else // Match angle based on time
        {
            // if during the day
            if (CLK_Hours >= 6 && CLK_Hours <= 18)
            {
                // calculate correct angle
                switch (CLK_Hours)
                {
                case (6):
                    pos = 0;
                    break;
                case (7):
                    pos = 15;
                    break;
                case (8):
                    pos = 30;
                    break;
                case (9):
                    pos = 45;
                    break;
                case (10):
                    pos = 60;
                    break;
                case (11):
                    pos = 75;
                    break;
                case (12):
                    pos = 90;
                    break;
                case (13):
                    pos = 105;
                    break;
                case (14):
                    pos = 120;
                    break;
                case (15):
                    pos = 135;
                    break;
                case (16):
                    pos = 150;
                    break;
                case (17):
                    pos = 165;
                    break;
                case (18):
                    pos = 180;
                default:
                    pos = 90;
                    break;
                }
            }
            else
            {
                // if during the night
                // rotate slowly to morning position and wait until day or until enough light
                if (pos >= 10)
                {
                    pos -= 10;
                }
            }
        }

        // Update timer.
        ControlTimer += CONTROL_INTERVAL;
    } // end of timer if
    // Clock Control

    if (millis() - ClkTimer >= CLOCK_TIME)
    {
        if (clockState == CLOCK_RUNNING)
        {
            UpdateClock();
        }
        ClkTimer += CLOCK_TIME;
        lcd.clear();
        SendClock(); // Place time
        // Set the cursor to indicate what is being set.
        switch (clockState)
        {
        case CLOCK_SET_HOURS:
            printAngle = false;
            lcd.setCursor(0, 0);
            break;
        case CLOCK_SET_MINUTES:
            lcd.setCursor(3, 0);
            break;
        case CLOCK_SET_SECONDS:
            lcd.setCursor(6, 0);
            break;
        case CLOCK_RUNNING:
            lcd.setCursor(0, 1);
            printAngle = true;
        }
        lcd.blink();
    }

    // Watch button and encoder to set clock
    if (ButtonNextState(digitalRead(4)) == 1) // Button pressed,
    {
        SettingClock(1, 0); // So update state.
    }
    if (clockState != CLOCK_RUNNING)
    {
        if ((encoderPosition - EncoderTracking) >= 4)
        {                       // The encoder has moved by one detent
            SettingClock(0, 1); // Indicate positive travel
            lcd.clear();
            SendClock();          // Place time
            EncoderTracking += 4; // and move tracking variable.
        }
        else if ((encoderPosition - EncoderTracking) <= -4)
        {                        // The encoder has moved by one detent in the opposite direction
            SettingClock(0, -1); // Indicate negative travel
            lcd.clear();
            SendClock();          // Place time
            EncoderTracking -= 4; // and Update Tracking accordingly
        }
    }

} // end of loop.
