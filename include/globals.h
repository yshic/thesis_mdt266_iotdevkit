/**
 * @file       globals.h
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for global
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef GLOBALS_H
  #define GLOBALS_H
  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

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
  #include "mini_fan.h"

  // Misc
  #include "bsp_gpio.h"
  #include "bsp_i2c.h"
  #include "button.h"
  #include "relay.h"
  #include "usb_switch.h"
  #include "utility.h"

// RTOS

  #include "../src/tasks/ble_task.h"
  #include "../src/tasks/sensors_task.h"
  #include "../src/tasks/wifi_task.h"

/* Public variables/objects ------------------------------------------- */
extern DHT20        dht20;
extern LCD_I2C      lcd;
extern LightSensor  lightSensor;
extern Ultrasonic   ultrasonic;
extern PIRSensor    pirSensor;
extern SoilMoisture soilMoisture;
extern MiniFan      miniFan;

// extern Relay         relay(D8);
// extern UsbSwitch     usbSwitch(D7, D6);
// extern ButtonHandler button(D2, false, true);

// WiFi credentials
extern char ssid[32];
extern char password[32];

#endif // GLOBALS_H

/* End of file -------------------------------------------------------- */