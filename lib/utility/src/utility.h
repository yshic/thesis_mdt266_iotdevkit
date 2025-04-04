/**
 * @file       utility.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Utility library. This library provides various utility functions for multiple
 * purposes.
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef UTILITY_H
  #define UTILITY_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "Wire.h"

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief  Scan for I2C devices and print the addresses to Serial.
 */
void scanI2CDevices(void);

/**
 * @brief  Scan for I2C devices and print the addresses to Serial.
 */
uint8_t crc8(const uint8_t *data, int len);

#endif // UTILITY_H

/* End of file -------------------------------------------------------- */