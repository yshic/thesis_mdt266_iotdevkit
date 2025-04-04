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

#ifdef MAIN
  #ifndef ENTERPRISE_WIFI
  bleSetup();
  #endif

  #ifdef WIFI_MODULE
  wifiSetup();
  #endif

  #ifdef DHT20_MODULE
  dht20Setup();
  #endif

  #ifdef SHT4X_MODULE
  sht40Setup();
  #endif

  #ifdef LIGHT_SENSOR_MODULE
  lightSensorSetup();
  #endif

  #ifdef ULTRASONIC_MODULE
  ultrasonicSetup();
  #endif

  #ifdef PIR_MODULE
  pirSensorSetup();
  #endif

  #ifdef SOIL_MOISTURE_MODULE
  soilMoistureSetup();
  #endif

  #ifdef LCD_MODULE
  lcdSetup();
  #endif

  #ifdef UNIT_4_RELAY_MODULE
  unit4RelaySetup();
  #endif

  #ifdef IOT_SERVER_MODULE
  iotServerSetup();
  #endif
#endif // MAIN

#ifdef DEBUG_I2C
  while (!Serial)
    ; // Wait for Serial to be ready
  scanI2CDevices();
#endif // DEBUG_I2C
}

void loop()
{
#ifdef DEBUG_I2C
  scanI2CDevices();
#endif

#ifdef MAIN
#endif
}