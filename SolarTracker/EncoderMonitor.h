/* EncoderMonitor.h
 * Author: Jacob Milburn
 * March 5, 2022
 */

volatile int encoderPosition = 0; //  Int positino that can change at any time

void EncoderMonitorA() //  ISR for Channel A
{
    if (digitalRead(2) == digitalRead(3))
    {
        encoderPosition++;
    }
    else
    {
        encoderPosition--;
    }
}

void EncoderMonitorB() //  ISR for Channel B
{
    if (digitalRead(2) == digitalRead(3))
    {
        encoderPosition--;
    }
    else
    {
        encoderPosition++;
    }
}

void EncoderInitialize() //  Starts encoder monitoring
{
    // Set pins as input
    pinMode(2, INPUT);
    pinMode(3, INPUT);

    // Attach interrupts to pins
    attachInterrupt(digitalPinToInterrupt(2), EncoderMonitorA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), EncoderMonitorB, CHANGE);
}