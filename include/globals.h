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

  /* Private defines ---------------------------------------------------- */
  // DEBUGGING
  // #define DEBUG_I2C
  #ifndef DEBUG_PRINT
    #define DEBUG_PRINT
  #endif // DEBUG_PRINT

// Communication
/* Wireless   --------------------------------------------------------- */
  // #define BLE_MODULE
  #define WIFI_MODULE
  // #define ENTERPRISE_WIFI
  #define IOT_SERVER_MODULE
  #define OTA_UPDATE_MODULE

/* Wired -------------------------------------------------------------- */
// #define UART_MODULE
// #define RS485_MODULE

// Peripherals

// Sensors
//  #define DHT20_MODULE
// #define UNIT_ENV_IV_MODULE
// #define SHT4X_MODULE
// #define BMP280_MODULE
  #define AC_MEASURE_MODULE
  #define LIGHT_SENSOR_MODULE
// #define PIR_MODULE
// #define SOIL_MOISTURE_MODULE
//  #define ULTRASONIC_MODULE

// Display
//  #define LCD_MODULE

// MOTORS / ACTUATORS
  #define MINI_FAN_MODULE
// #define BUTTON_MODULE
// #define RELAY_MODULE
  #define UNIT_4_RELAY_MODULE
// #define USB_SWITCH_MODULE

  #ifdef UNIT_ENV_IV_MODULE
    #ifndef SHT4X_MODULE
      #define SHT4X_MODULE
    #endif

    #ifndef BMP280_MODULE
      #define BMP280_MODULE
    #endif
  #endif // UNIT_ENV_IV_MODULE

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/**** Custom libraries ****/
  // Display
  #ifdef LCD_MODULE
    #include "LCD-I2C.h"
extern LCD_I2C lcd;
  #endif // LCD_MODULE

  // Sensors
  #ifdef DHT20_MODULE
    #include "dht20.h"
extern DHT20 dht20;
  #endif // DHT20_MODULE

  #ifdef SHT4X_MODULE
    #include "sht4x.h"
extern SHT4X sht40;
  #endif // SHT4X_MODULE

  #ifdef BMP280_MODULE
    #include "bmp280.h"
extern BMP280 bmp280;
  #endif // BMP280_MODULE

  #ifdef AC_MEASURE_MODULE
    #include "ac_measure.h"
extern AcMeasure acMeasure;
  #endif // AC_MEASURE_MODULE

  #ifdef LIGHT_SENSOR_MODULE
    #include "light_sensor.h"
extern LightSensor lightSensor;
  #endif // LIGHT_SENSOR_MODULE

  #ifdef PIR_MODULE
    #include "pir_sensor.h"
extern PIRSensor pirSensor;
  #endif // PIR_MODULE

  #ifdef SOIL_MOISTURE_MODULE
    #include "soil_moisture.h"
extern SoilMoisture soilMoisture;
  #endif // SOIL_MOISTURE_MODULE

  #ifdef ULTRASONIC_MODULE
    #include "ultrasonic.h"
extern Ultrasonic ultrasonic;
  #endif // ULTRASONIC_MODULE

  // Motor
  #ifdef MINI_FAN_MODULE
    #include "mini_fan.h"
extern MiniFan miniFan;
  #endif // MINI_FAN_MODULE

  // Misc
  #include "bsp_gpio.h"
  #include "bsp_i2c.h"
  #include "bsp_uart.h"

  #ifdef BUTTON_MODULE
    #include "button.h"
extern ButtonHandler button;
  #endif // BUTTON_MODULE

  #ifdef RELAY_MODULE
    #include "relay.h"
extern Relay relay(D8);
  #endif // RELAY_MODULE

  #ifdef UNIT_4_RELAY_MODULE
    #include "unit_4relay.h"
extern Unit4Relay unit4Relay;
  #endif // UNIT_4_RELAY_MODULE

  #ifdef USB_SWITCH_MODULE
    #include "usb_switch.h"
extern UsbSwitch usbSwitch(D7, D6);
  #endif // USB_SWITCH_MODULE

  #include "secrets.h"
  #include "utility.h"

// RTOS
  #include "../src/tasks/ble_task.h"
  #include "../src/tasks/button_task.h"
  #include "../src/tasks/iot_server_task.h"
  #include "../src/tasks/lcd_task.h"
  #include "../src/tasks/relay_task.h"
  #include "../src/tasks/sensors_task.h"
  #include "../src/tasks/uart_task.h"
  #include "../src/tasks/wifi_task.h"

  // WiFi credentials
  #ifndef ENTERPRISE_WIFI
extern char ssid[32];
extern char password[32];
  #endif // ENTERPRISE_WIFI

extern bool wifiConnected;

#endif // GLOBALS_H

/* End of file -------------------------------------------------------- */