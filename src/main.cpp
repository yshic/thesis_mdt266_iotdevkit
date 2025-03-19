/* Includes ----------------------------------------------------------- */
#include "globals.h"

/* Public defines ----------------------------------------------------- */
// DEBUGGING
// #define DEBUG_I2C

// Main code
#ifndef DEBUG_I2C
  #define MAIN
#endif

/* Forward function declaration --------------------------------------- */

/* Public variables/object --------------------------------------------------- */

/* Public Class/Struct     --------------------------------------------------- */

/* Function definitions --------------------------------------- */

/* Main code ----------------------------------------------------- */
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  bspGpioPinMode(LED_BUILTIN, OUTPUT);
  bspGpioDigitalWrite(LED_BUILTIN, HIGH);

#ifdef MAIN
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.clear();

  bleSetup();
  wifiSetup();

  dht20Setup();
  lightSensorSetup();
  ultrasonicSetup();
  pirSensorSetup();
  soilMoistureSetup();
#endif

#ifdef DEBUG_I2C
  while (!Serial)
    ; // Wait for Serial to be ready
  scanI2CDevices();
#endif
}

void loop()
{
#ifdef DEBUG_I2C
  scanI2CDevices();
#endif

#ifdef MAIN
#endif
}