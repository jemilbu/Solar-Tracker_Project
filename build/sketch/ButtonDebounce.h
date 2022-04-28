/* ButtonDebounce.h
 * Author: Jacob Milburn
 * February 27, 2022
 */

enum ButtonStates
{
    ButtonIdle,
    ButtonWait,
    ButtonLow
} ButtonState;            // Global variable holding status of button
unsigned long ButtonTime; // unsigned long to hold time to debounce Switch

int ButtonInitialize() // function to set up system to read button presses
{
    pinMode(4, INPUT);
    ButtonState = ButtonIdle;
    pinMode(13, OUTPUT); // used for debugging

} // end of ButtonInitialize

int ButtonNextState(int InputPin) // function that is to be called in loop to service the switch
{
    switch (ButtonState)
    {
    case ButtonIdle: // State where nothing has been happening
        if (InputPin == LOW)
        {
            ButtonTime = millis();    // Record time of high to low transition
            ButtonState = ButtonWait; // Move to Wait state
            digitalWrite(13, HIGH);   // Pin 13 high ( used for debugging )
        }
        break;
    case ButtonWait:          // Button has gone low and we are waiting for it to remain low for 5 m seconds
        if (InputPin == HIGH) // If button has gone high,
        {
            ButtonState = ButtonIdle; // Reset back to Idle
            digitalWrite(13, LOW);    // Pin 13 low ( used for debugging )
        }
        else if (millis() - ButtonTime >= 5) // if 5 milliseconds has passed
        {
            ButtonState = ButtonLow; // Move to low state
            digitalWrite(13, LOW);   // Pin 13 low ( used for debugging )
            return 1;                // indicating that button has been pressed
        }
        break;
    case ButtonLow:           // Button is low and has been so for 5 milliseconds
        if (InputPin == HIGH) // Once button released
        {
            ButtonState = ButtonIdle; // Go back to Idle state
            return 2;                 // indicate that the button has been released
        }
        break;
    }
    return 0; // By default return 0 indicating nothing is happening
} // End of Button Next state
