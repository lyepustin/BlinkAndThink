#define TdsSensorPin 34
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT 30     // sum of sample point
#define ADCRANGE 2750 // 1024.0
#define KVALUE 0.4    // 0.5

const float VOLTAGE_CONSTANT = VREF / ADCRANGE;

int analogBuffer[SCOUNT]; // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0, copyIndex = 0;
float averageVoltage = 0, tdsValue = 0;

float temperature = 20;
float median_raw = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    static unsigned long printTimepoint = millis();
    if (millis() - printTimepoint > 800U)
    {
        printTimepoint = millis();

        int raw_reading = analogRead(TdsSensorPin);
        Serial.print("  raw:");
        Serial.print(raw_reading);

        float measured_voltage = VOLTAGE_CONSTANT * raw_reading;
        Serial.print("   measured_voltage:");
        Serial.print(measured_voltage);

        float compensationCoefficient = (1.0 - (0.02 * (temperature - 25.0)));                                                                                                              // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
        float compensationVoltage = measured_voltage / compensationCoefficient;                                                                                                             // temperature compensation
        tdsValue = (133.42 * compensationVoltage * compensationVoltage * compensationVoltage - 255.86 * compensationVoltage * compensationVoltage + 857.39 * compensationVoltage) * KVALUE; // convert voltage value to tds value

        Serial.print("V   ");
        Serial.print("   compensationCoefficient:");
        Serial.print(compensationCoefficient);
        Serial.print("   compensationVoltage:");
        Serial.print(compensationVoltage);
        Serial.print("   TDS Value:");
        Serial.print(tdsValue);
        Serial.println("ppm");
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