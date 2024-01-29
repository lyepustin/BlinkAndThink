#include <Wire.h>

// LiquidCrystal_I2C lcd(0x3F,16,2); //Set 0x27 if your using different i2c address (20,4 if your using 20/4 LCD)
int pHSense = 35;
float voltage = 3.3;
float adc_resolution = 4095.0;

// 0 at 0V
// 4095 at 3.3V
// 2072 at 1.67V (which is 1.67/3.3 * 4095)

// 2048 -> 7ph
// 2340 -> 8ph
// 2633 -> 9ph

void setup()
{

    Serial.begin(9600);
    delay(100);
    // Wire.begin(2,14);
    // lcd.init();
    // lcd.backlight();
    // lcd.setCursor(0,0);
    // lcd.print("14CORE | pH Probe ");
    // lcd.setCursor(0,1);
    // lcd.print(">Initializing...");
    // delay(4000);
    //  lcd.clear();
    // lcd.setCursor(0,0);
    // lcd.print("14CORE | pH Sense");
    Serial.println("14CORE | pH Sense");
}

void loop()
{
    int measuringVal = analogRead(pHSense);
    Serial.print("Measuring Raw Value > ");
    Serial.println(measuringVal);

    double pHVol = voltage / adc_resolution * measuringVal;
    Serial.print("pH voltage Value > ");
    Serial.println(pHVol, 3);

    // float P0 = 7 + ((2.5 - pHVol) / 0.18);
    // Serial.print("");
    // Serial.print("pH Value > ");
    // Serial.print(P0, 3);
    // Serial.println("");

    // float phValue = -5.70 * pHVol + 21.34;
    // Serial.print("pH Value v2 = ");
    // Serial.println(phValue);

    // float phValuev3 = (-pHVol + 3.76) / 0.18;
    // Serial.print("pH Value v3 = ");
    // Serial.println(phValuev3);

    delay(3000);
}