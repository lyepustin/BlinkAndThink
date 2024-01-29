/*----------------------------------------------------------
    MH-Z19 CO2 sensor  SAMPLE
    https://github.com/WifWaf/MH-Z19
  ----------------------------------------------------------*/

#include <Arduino.h>
#include "MHZ19.h"

#define MHZ19_BAUDRATE 9600 // Device to MH-Z19 Serial baudrate (should not be changed)
// Serial Port connections for Sensor
#define RXD2 16 // To sensor TXD
#define TXD2 17 // To sensor RXD

MHZ19 myMHZ19; // Constructor for library
unsigned long getDataTimer = 0;

void setup()
{
    Serial.begin(115200); // Device to serial monitor feedback
    Serial1.begin(MHZ19_BAUDRATE, SERIAL_8N1, RXD2, TXD2);
    myMHZ19.begin(Serial1);    // *Serial(Stream) reference must be passed to library begin().
    myMHZ19.autoCalibration(); // Turn auto calibration ON (OFF autoCalibration(false))
}

void loop()
{
    if (millis() - getDataTimer >= 2000)
    {
        int CO2;

        /* note: getCO2() default is command "CO2 Unlimited". This returns the correct CO2 reading even
        if below background CO2 levels or above range (useful to validate sensor). You can use the
        usual documented command with getCO2(false) */

        CO2 = myMHZ19.getCO2(); // Request CO2 (as ppm)

        Serial.print("CO2 (ppm): ");
        Serial.println(CO2);

        int8_t Temp;
        Temp = myMHZ19.getTemperature(); // Request Temperature (as Celsius)
        Serial.print("Temperature (C): ");
        Serial.println(Temp);

        getDataTimer = millis();
    }
}