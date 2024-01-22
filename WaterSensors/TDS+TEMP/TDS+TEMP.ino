#include <OneWire.h>
#include <DallasTemperature.h>

#define TdsSensorPin 34
#define DS18B20_Pin 13 // water temp sensor
#define VREF 5.0       // analog reference voltage(Volt) of the ADC
#define SCOUNT 30      // sum of sample point

int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0;

OneWire ourWire(DS18B20_Pin); // Pin 13 is set as the OneWire bus for DS18B20 sensor
DallasTemperature sensors(&ourWire);

void setup()
{
    delay(1000);
    Serial.begin(115200);
    pinMode(TdsSensorPin, INPUT);
    sensors.begin();
}

void loop()
{
    static unsigned long analogSampleTimepoint = millis();
    if (millis() - analogSampleTimepoint > 40U) // every 40 milliseconds, read the analog value from the ADC
    {
        analogSampleTimepoint = millis();
        analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin); // read the analog value and store into the buffer
        analogBufferIndex++;
        if (analogBufferIndex == SCOUNT)
            analogBufferIndex = 0;
    }

    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 800U) // every 800 milliseconds, print values
    {
        printTimepoint = millis();
        for (copyIndex = 0; copyIndex < SCOUNT; copyIndex++)
            analogBufferTemp[copyIndex] = analogBuffer[copyIndex];

        averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value

        sensors.requestTemperatures();                  // Send command to retrieve the temperature of the DS18B20 sensor
        float temperature = sensors.getTempCByIndex(0); // Get the temperature from DS18B20 sensor in ºC

        float compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0);                                                                                                               // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVolatge = averageVoltage / compensationCoefficient;                                                                                                            // temperature compensation
        tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; // convert voltage value to tds value

        Serial.print("TDS ppm value=");
        Serial.print(tdsValue, 0);

        float microSiemenValue = tdsValue * 1.56;

        Serial.print("| TDS microS/cm value=");
        Serial.print(microSiemenValue, 0);

        Serial.print("| Temperature value=");
        Serial.print(temperature);
        Serial.println(" ºC");
    }
}

int getMedianNum(int bArray[], int iFilterLen)
{
    int bTab[iFilterLen];
    for (byte i = 0; i < iFilterLen; i++)
        bTab[i] = bArray[i];

    int i, j, bTemp;
    for (j = 0; j < iFilterLen - 1; j++)
    {
        for (i = 0; i < iFilterLen - j - 1; i++)
        {
            if (bTab[i] > bTab[i + 1])
            {
                bTemp = bTab[i];
                bTab[i] = bTab[i + 1];
                bTab[i + 1] = bTemp;
            }
        }
    }

    if ((iFilterLen & 1) > 0)
        bTemp = bTab[(iFilterLen - 1) / 2];
    else
        bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;

    return bTemp;
}
