/**
 * @file       smart_home.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-28
 * @author     Tuan Nguyen
 *
 * @brief      Header file for SMART_HOME library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SMART_HOME_H
  #define SMART_HOME_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define SMART_HOME_LIB_VERSION    (F("0.1.0"))

/* Device index mappings */
  #define DEVICE_INDEX_ENV_IV       0
  #define DEVICE_INDEX_LIGHT_SENSOR 1
  #define DEVICE_INDEX_PIR_SENSOR   2
  #define DEVICE_INDEX_ES_SOIL      3
  #define DEVICE_INDEX_LCD          4
  #define DEVICE_INDEX_4_RELAY      5
  #define DEVICE_INDEX_BUTTON       6
  #define DEVICE_INDEX_SERVO        7
  #define DEVICE_INDEX_MINI_FAN     8
  #define DEVICE_INDEX_AC_MEASURE   9
  #define DEVICE_INDEX_HUSKYLENS    10

  #define CONNECTIVITY_WIFI_INDEX   0
  #define CONNECTIVITY_BLE_INDEX    1

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class SmartHome
{
public:
  /**
   * @brief  Initializes the serial interfaces (UART, RS-485, and/or I2C) based on configuration.
   *
   * This function initializes the necessary serial communication interfaces for the Smart Home system.
   * It begins the Serial monitor at 9600 baud rate and conditionally initializes UART, RS-485, and I2C
   * based on the user-provided flags.
   *
   * @param[in]   uart    Boolean flag to initialize UART interface.
   * @param[in]   rs485   Boolean flag to initialize RS-485 interface.
   * @param[in]   i2c     Boolean flag to initialize I2C interface.
   *
   * @attention  Ensure that hardware peripherals associated with UART, RS-485, and I2C are correctly
   *             connected and powered on before calling this function. I2C will call Wire.begin().
   *
   * @return     None
   */
  void serialSetup(bool uart, bool rs485, bool i2c);

  /**
   * @brief  Enables or disables devices based on a user-provided status array.
   *
   * This function updates the internal device status array used for selective device initialization
   * and runtime control. Each device corresponds to a specific predefined index.
   *
   * @param[in]   status   Boolean array of 11 elements representing the enable (true) or disable (false)
   *                       state for each device.
   *
   * The device indices are mapped as follows:
   * 
   *  - 0: ENVIV (Environmental Sensor - SHT40, BMP280)
   *
   *  - 1: Light Sensor
   *
   *  - 2: PIR Motion Sensor
   *
   *  - 3: ES Soil 7-in-1 Sensor (RS-485)
   *
   *  - 4: LCD 16x2 Display
   *
   *  - 5: Unit 4-Relay Module
   *
   *  - 6: Button
   *
   *  - 7: Servo Motor
   *
   *  - 8: Mini Fan
   *
   *  - 9: AC Measurement Module
   *
   *  - 10: Huskylens (Vision Sensor)
   *
   * @attention  Ensure the array passed contains exactly 11 elements in the correct order as shown above.
   *
   * @return     None
   */
  void deviceEnable(const bool status[11]);

  /**
   * @brief  Enables or disables connectivity modules based on user-provided status array.
   *
   * This function updates the internal connectivity status list used to manage Wi-Fi and BLE setup.
   *
   * @param[in]   status   Boolean array of 2 elements representing Wi-Fi and BLE enable/disable state.
   *
   * @attention  Ensure the array passed contains exactly 2 elements corresponding to Wi-Fi and BLE indices.
   *
   * @return     None
   */
  void connectivityEnable(const bool status[2]);

  /**
   * @brief  Initializes all enabled devices.
   *
   * This function iterates through the list of devices and initializes only those devices
   * that have been marked as enabled in the internal status array.
   *
   * @param[in]   None
   *
   * @attention  Device enable statuses must be set beforehand by calling deviceEnable().
   *
   * @return     None
   */
  void deviceSetup();

  /**
   * @brief  Initializes all enabled connectivity modules.
   *
   * This function initializes Wi-Fi or BLE modules if they have been enabled
   * using the connectivityEnable() function.
   *
   * @param[in]   None
   *
   * @attention  Connectivity enable statuses must be configured prior to calling this function.
   *
   * @return     None
   */
  void connectivitySetup();

  /**
   * @brief  Connects the device to the configured IoT server.
   *
   * This function performs setup routines necessary to establish a connection
   * with an IoT server for data publishing or remote monitoring.
   *
   * @param[in]   None
   *
   * @attention  Network configuration and initialization must be completed before calling this function.
   *
   * @return     None
   */
  void iotServerConnect();

private:
  bool deviceStatus[11] = {0};
  bool connectivity[2]  = {0};

private:
  /**
   * @brief  Initializes an individual device based on its index.
   *
   * This function maps a given device index to its corresponding hardware initialization function.
   *
   * @param[in]   index   The device index corresponding to predefined macros (e.g., DEVICE_INDEX_ENV_IV).
   *
   * @attention  This function is normally called internally by deviceSetup() and not intended for direct use.
   *
   * @return     None
   */
  void deviceInitialize(uint8_t index);

  /**
   * @brief  Initializes an individual connectivity module based on its index.
   *
   * This function maps a given connectivity index (Wi-Fi or BLE) to the corresponding setup function.
   *
   * @param[in]   index   The connectivity index (e.g., CONNECTIVITY_WIFI_INDEX).
   *
   * @attention  This function is usually invoked internally by connectivitySetup().
   *
   * @return     None
   */
  void connectivityInitialize(uint8_t index);
};

#endif // SMART_HOME_H

/* End of file -------------------------------------------------------- */