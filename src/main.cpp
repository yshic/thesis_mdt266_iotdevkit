/* Includes ----------------------------------------------------------- */
#include <Arduino.h>
#include <Wire.h>

/**** Custom libraries ****/

// Display
#include "LCD-I2C.h"

// Sensors
#include "dht20.h"
#include "light_sensor.h"
#include "pir_sensor.h"
#include "soil_moisture.h"
#include "ultrasonic.h"

// Motor
// #include "mini_fan.h"

// Misc
#include "relay.h"
#include "usb_switch.h"
#include "utility.h"

/* Public variables/object --------------------------------------------------- */
// Objects
LCD_I2C     lcd(0x21, 16, 2);
DHT20       dht20;
LightSensor lightSensor(A0);
Ultrasonic  ultrasonic(D8, D9);
// SoilMoisture soilMoisture(A8);
// MiniFan miniFan(A8);
// Relay relay(D8);

UsbSwitch usbSwitch(D7, D6);

// Variables
int state = 0;

/* Public defines ----------------------------------------------------- */
// DEBUGGING
// #define DEBUG_I2C

// Main code
#ifndef DEBUG_I2C
  #define MAIN

  // Display
  #define LCD_MODULE

  // Sensors
  #define DHT20_MODULE
  #define LIGHT_MODULE
// #define MOISTURE_MODULE

// Motor
// #define MINI_FAN_MODULE

// Misc
// #define RELAY_MODULE
#endif

/* Main code ----------------------------------------------------- */
void setup()
{
  Serial.begin(9600);
  Wire.begin();

#ifdef MAIN
  #ifdef DHT20_MODULE
  // dht20.begin();
  #endif

  #ifdef LCD_MODULE
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.clear();
  #endif
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
  float sensorDHTValues[2] = {0};

  switch (state)
  {
    case 0: {
      state = 1;
  #ifdef DHT20_MODULE
      if (!dht20.readTempAndHumidity(sensorDHTValues))
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Hum: ");
        lcd.print(sensorDHTValues[0]);
        lcd.print(" %");
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(sensorDHTValues[1]);
        lcd.print(" *C");
      }
      else
      {
        lcd.clear();
        lcd.print("ERROR");
      }
  #endif
      break;
    }
    case 1: {
      state = 2;
  #ifdef LIGHT_MODULE
      int sensorLightValues = lightSensor.readAndMap(0, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Light level: ");
      lcd.print(sensorLightValues);
      lcd.progressBar(1, sensorLightValues);
  #endif
      break;
    }
    case 2: {
      state = 3;
  #ifdef MOISTURE_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Moisture: ");
      lcd.print(soilMoisture.read());
      lcd.setCursor(0, 1);
      lcd.print(soilMoisture.readAndMap(0, 100));
      lcd.print(" %");
  #endif
      break;
    }
    case 3: {
      state = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      lcd.print(ultrasonic.read('c'));
      lcd.print(" cm");
      break;
    }
    default:
      break;
  }
  delay(2000);
#endif
}