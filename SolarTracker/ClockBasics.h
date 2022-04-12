#ifndef ClockBasics_H
#define ClockBasics_H
// Variable used as clock settings.
int CLK_Hours, CLK_Minutes, CLK_Seconds;

//  Headers to include
#include <LiquidCrystal.h>
LiquidCrystal ClockLcdDriver(A5, A4, 5, 6, 7, 8);


// This function is to be called every second
// to update the clock represented by the
// global variables Hours, Minutes, Seconds
void UpdateClock()
{
    // Check if Seconds not at wrap point.
    if (CLK_Seconds < 59)
    {
        CLK_Seconds++; // Move seconds ahead.
    }
    else
    {
        CLK_Seconds = 0; // Reset Seconds
        // and check Minutes for wrap.
        if (CLK_Minutes < 59)
        {
            CLK_Minutes++; // Move seconds ahead.
        }
        else
        {
            CLK_Minutes = 0; // Reset Minutes
            // check Hours for wrap
            if (CLK_Hours < 23)
            {
                CLK_Hours++; // Move Hours ahead.
            }
            else
            {
                CLK_Hours = 0; // Reset Hours
            }                  // End of CLK_Hours test.
        }                      // End of Minutes test
    }                          // End of Seconds test
} // end of UpdateClock()
// Send Hours, Minutes and Seconds to a display.
void SendClock()
{
    //ClockLcdDriver.print(" "); // Position over by one.
    // Check if leading zero needs to be sent
    if (CLK_Hours < 10)
    {
        ClockLcdDriver.print("0");
    }
    ClockLcdDriver.print(CLK_Hours); // Then send hours
    ClockLcdDriver.print(":");       // And separator
    // Check for leading zero on Minutes.
    if (CLK_Minutes < 10)
    {
        ClockLcdDriver.print("0");
    }
    ClockLcdDriver.print(CLK_Minutes); // Then send Minutes
    ClockLcdDriver.print(":");         // And separator
    // Check for leading zero needed for Seconds.
    if (CLK_Seconds < 10)
    {
        ClockLcdDriver.print("0");
    }
    ClockLcdDriver.print(CLK_Seconds); // Then send Seconds
} // End of SendClock()
// States for setting clock.
enum ClockStates
{
    CLOCK_RUNNING,
    CLOCK_SET_HOURS,
    CLOCK_SET_MINUTES,
    CLOCK_SET_SECONDS
};
ClockStates clockState = CLOCK_RUNNING;
// Function that processes incoming characters to set the clock.
void SettingClock(int Button, int UpDown)
{
    // interpret input based on state
    switch (clockState)
    {
    case CLOCK_RUNNING:
        if (Button)
        {
            clockState = CLOCK_SET_HOURS;
            CLK_Hours = 0; // Reset clock values.
            CLK_Minutes = 0;
            CLK_Seconds = 0;
        }
        break;
    case CLOCK_SET_HOURS: // Setting hours
        if (Button)       // if button was pressed,
        {
            clockState = CLOCK_SET_MINUTES; // move to minutes.
        }
        else
        {
            CLK_Hours += UpDown; // Adjust hours,
            if (CLK_Hours > 23)  // Wrap at 23 and 0.
                CLK_Hours = 0;
            if (CLK_Hours < 0)
                CLK_Hours = 23;
        }
        break;
    case CLOCK_SET_MINUTES: // Repeat above for minutes.
        if (Button)
        {
            clockState = CLOCK_SET_SECONDS;
        }
        else
        {
            CLK_Minutes += UpDown;
            if (CLK_Minutes > 59)
                CLK_Minutes = 0;
            if (CLK_Minutes < 0)
                CLK_Minutes = 59;
        }
        break;
    case CLOCK_SET_SECONDS: // Repeat for seconds.
        if (Button)
        {
            clockState = CLOCK_RUNNING;
        }
        else
        {
            CLK_Seconds += UpDown;
            if (CLK_Seconds > 59)
                CLK_Seconds = 0;
            if (CLK_Seconds < 0)
                CLK_Seconds = 59;
        }
        break;
    } // End of clock mode switch.
} // End of SettingClock
#endif