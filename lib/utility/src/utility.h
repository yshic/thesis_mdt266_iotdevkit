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
 * @brief  Calculates CRC-8 checksum for the given data using SHT40's polynomial.
 *
 * This function computes an 8-bit CRC checksum using the polynomial defined in the SHT40
 * datasheet. It is used to verify data integrity when reading from the SHT40 sensor.
 *
 * The CRC-8 parameters are:
 *   - Initialization value: 0xFF
 *   - Polynomial: 0x31 (x⁸ + x⁵ + x⁴ + 1)
 *   - No final XOR (0x00)
 *
 * Example from datasheet: For data bytes {0xBE, 0xEF}, the CRC result is 0x92.
 *
 * @param[in]     data  Pointer to the input data buffer.
 * @param[in]     len   Length of the data buffer in bytes.
 *
 * @attention  Ensure that `data` points to a valid memory location of at least `len` bytes.
 *             This function does not perform bounds checking.
 *
 * @return
 *  - CRC-8 checksum as an 8-bit unsigned integer.
 */
uint8_t crc8(const uint8_t *data, int len);

/**
 * @brief  Compares two firmware version strings.
 *
 * This function compares two version strings (e.g., "1.0", "0.9.3", "2") by parsing up to three levels
 * of semantic versioning: major, minor, and patch. Missing segments are treated as zero, allowing
 * flexible comparison of version formats like "0.1", "0.1.0", or "1".
 *
 * @param[in]     v1  The current firmware version (as a `String`).
 * @param[in]     v2  The incoming/target firmware version (as a `String`).
 *
 * @attention  Version strings should be in numeric dot-separated format. Non-numeric parts are ignored.
 *             For example, "1.0a" would parse as "1.0.0". Only the first three segments are considered.
 *
 * @return
 *  - `-1` if `v1` is lower than `v2`
 *  - ` 0` if both versions are equal
 *  - ` 1` if `v1` is greater than `v2`
 */
int compareVersion(String v1, String v2);

#endif // UTILITY_H

/* End of file -------------------------------------------------------- */