/**
 * @file       ac_measure.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-28
 * @author     Tuan Nguyen
 *
 * @brief      Header file for AC_MEASURE library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef AC_MEASURE_H
  #define AC_MEASURE_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define AC_MEASURE_LIB_VERSION                   (F("0.1.0"))

  #define UNIT_ACMEASURE_DEFAULT_ADDR              0x42
  #define UNIT_ACMEASURE_VOLTAGE_STRING_REG        0x00
  #define UNIT_ACMEASURE_CURRENT_STRING_REG        0x10
  #define UNIT_ACMEASURE_POWER_STRING_REG          0x20
  #define UNIT_ACMEASURE_APPARENT_POWER_STRING_REG 0x30
  #define UNIT_ACMEASURE_POWER_FACTOR_STRING_REG   0x40
  #define UNIT_ACMEASURE_KWH_STRING_REG            0x50
  #define UNIT_ACMEASURE_VOLTAGE_REG               0x60
  #define UNIT_ACMEASURE_CURRENT_REG               0x70
  #define UNIT_ACMEASURE_POWER_REG                 0x80
  #define UNIT_ACMEASURE_APPARENT_POWER_REG        0x90
  #define UNIT_ACMEASURE_POWER_FACTOR_REG          0xA0
  #define UNIT_ACMEASURE_KWH_REG                   0xB0
  #define UNIT_ACMEASURE_VOLTAGE_FACTOR_REG        0xC0
  #define UNIT_ACMEASURE_CURRENT_FACTOR_REG        0xD0
  #define UNIT_ACMEASURE_SAVE_FACTOR_REG           0xE0
  #define UNIT_ACMEASURE_GET_READY_REG             0xFC
  #define UNIT_ACMEASURE_JUMP_TO_BOOTLOADER_REG    0xFD
  #define UNIT_ACMEASURE_FIRMWARE_VERSION_REG      0xFE
  #define UNIT_ACMEASURE_I2C_ADDRESS_REG           0xFF

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  UNIT_AC_MEASURE_OK = 0,   /* No error */
  UNIT_AC_MEASURE_ERR,      /* Generic error */
  UNIT_AC_MEASURE_ERR_INIT, /* Initialization error */
  UNIT_AC_MEASURE_TIMEOUT,  /* Timeout error*/
  UNIT_AC_MEASURE_ERR_I2C,  /* I2C error */
} ac_measure_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages AC measurement operations over I2C, providing access to voltage, current, power, and energy
 * data.
 *
 * The `AcMeasure` class facilitates interaction with an AC measurement device via I2C communication.
 * It supports reading raw and converted electrical parameters such as voltage, current, power, apparent
 * power, power factor, and energy consumption. Additionally, it allows configuration of calibration factors
 * and I2C address.
 *
 * ### Features:
 *
 * - Reads raw and converted values for voltage, current, power, and energy.
 *
 * - Supports string representations of measurement data.
 *
 * - Configurable voltage and current calibration factors.
 *
 * - I2C address management and bootloader mode access.
 *
 * - Firmware version and device readiness checks.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` to initialize the device. Use the various getter methods to
 * retrieve measurement data and setter methods to configure the device. Ensure periodic I2C communication
 * checks for reliable operation.
 *
 * ### Dependencies:
 *
 * - Requires I2C communication library (`bspI2C` functions).
 *
 * - Device must be connected to a valid I2C bus.
 *
 * - Calibration factors should be set appropriately for accurate measurements.
 */
class AcMeasure
{
private:
  uint8_t _addr = UNIT_ACMEASURE_DEFAULT_ADDR;

public:
  /**
   * @brief Initializes the AC measurement device by checking I2C connectivity.
   *
   * This function verifies the presence of the device at the specified I2C address.
   * It must be called before any other operations to ensure proper communication.
   *
   * @param[in] None
   *
   * @attention Ensure the device is powered and correctly connected to the I2C bus.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   *
   *  - `UNIT_AC_MEASURE_ERR_I2C`: I2C communication failure
   */
  ac_measure_error_t begin(void);

  /**
   * @brief Retrieves the firmware version of the AC measurement device.
   *
   * Reads the firmware version from the device’s firmware version register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The firmware version.
   */
  uint8_t getFirmwareVersion(void);

  /**
   * @brief Checks the readiness status of the AC measurement device.
   *
   * Queries the device to determine if it is ready to provide measurement data.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The readiness status (typically 0 or 1).
   */
  uint8_t getReady(void);

  /**
   * @brief Reads the raw voltage value from the device.
   *
   * Retrieves the unprocessed voltage data from the device’s voltage register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint16_t The raw voltage value.
   */
  uint16_t getRawVoltage(void);

  /**
   * @brief Retrieves the voltage in volts.
   *
   * Converts the raw voltage value to a floating-point representation in volts.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The voltage value in volts.
   */
  float getVoltage(void);

  /**
   * @brief Reads the raw current value from the device.
   *
   * Retrieves the unprocessed current data from the device’s current register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint16_t The raw current value.
   */
  uint16_t getRawCurrent(void);

  /**
   * @brief Retrieves the current in amperes.
   *
   * Converts the raw current value to a floating-point representation in amperes.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The current value in amperes.
   */
  float getCurrent(void);

  /**
   * @brief Reads the raw power value from the device.
   *
   * Retrieves the unprocessed power data from the device’s power register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint32_t The raw power value.
   */
  uint32_t getRawPower(void);

  /**
   * @brief Retrieves the power in watts.
   *
   * Converts the raw power value to a floating-point representation in watts.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The power value in watts.
   */
  float getPower(void);

  /**
   * @brief Reads the raw apparent power value from the device.
   *
   * Retrieves the unprocessed apparent power data from the device’s apparent power register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint32_t The raw apparent power value.
   */
  uint32_t getRawApparentPower(void);

  /**
   * @brief Retrieves the apparent power in volt-amperes.
   *
   * Converts the raw apparent power value to a floating-point representation in volt-amperes.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The apparent power value in volt-amperes.
   */
  float getApparentPower(void);

  /**
   * @brief Reads the raw power factor value from the device.
   *
   * Retrieves the unprocessed power factor data from the device’s power factor register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The raw power factor value.
   */
  uint8_t getRawPowerFactor(void);

  /**
   * @brief Retrieves the power factor.
   *
   * Converts the raw power factor value to a floating-point representation.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The power factor value.
   */
  float getPowerFactor(void);

  /**
   * @brief Reads the raw energy consumption value from the device.
   *
   * Retrieves the unprocessed energy consumption data from the device’s KWH register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization transact()`.
   *
   * @return uint32_t The raw energy consumption value in watt-hours.
   */
  uint32_t getRawKWH(void);

  /**
   * @brief Retrieves the energy consumption in kilowatt-hours.
   *
   * Converts the raw energy consumption value to a floating-point representation in kilowatt-hours.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return float The energy consumption value in kilowatt-hours.
   */
  float getKWH(void);

  /**
   * @brief Retrieves the voltage as a formatted string.
   *
   * Reads the voltage data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the voltage string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 7 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getVoltageString(char *str);

  /**
   * @brief Retrieves the current as a formatted string.
   *
   * Reads the current data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the current string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 7 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getCurrentString(char *str);

  /**
   * @brief Retrieves the power as a formatted string.
   *
   * Reads the power data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the power string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 7 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getPowerString(char *str);

  /**
   * @brief Retrieves the apparent power as a formatted string.
   *
   * Reads the apparent power data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the apparent power string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 7 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getApparentPowerString(char *str);

  /**
   * @brief Retrieves the power factor as a formatted string.
   *
   * Reads the power factor data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the power factor string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 4 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getPowerFactorString(char *str);

  /**
   * @brief Retrieves the energy consumption as a formatted string.
   *
   * Reads the energy consumption data from the device and stores it as a string in the provided buffer.
   *
   * @param[out] str Pointer to the character array to store the energy consumption string.
   *
   * @attention The provided buffer must be large enough to hold the string (typically 11 bytes).
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t getKWHString(char *str);

  /**
   * @brief Sets the energy consumption value on the device.
   *
   * Writes the specified energy consumption value to the device’s KWH register.
   *
   * @param[in] value The energy consumption value in watt-hours to set.
   *
   * @attention Ensure the value is within the valid range for the device.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t setKWH(uint32_t value);

  /**
   * @brief Retrieves the voltage calibration factor.
   *
   * Reads the voltage calibration factor from the device’s voltage factor register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The voltage calibration factor.
   */
  uint8_t getVoltageFactor(void);

  /**
   * @brief Retrieves the current calibration factor.
   *
   * Reads the current calibration factor from the device’s current factor register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The current calibration factor.
   */
  uint8_t getCurrentFactor(void);

  /**
   * @brief Sets the voltage calibration factor.
   *
   * Writes the specified voltage calibration factor to the device’s voltage factor register.
   *
   * @param[in] value The voltage calibration factor to set.
   *
   * @attention Ensure the value is within the valid range for the device.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t setVoltageFactor(uint8_t value);

  /**
   * @brief Sets the current calibration factor.
   *
   * Writes the specified current calibration factor to the device’s current factor register.
   *
   * @param[in] value The current calibration factor to set.
   *
   * @attention Ensure the value is within the valid range for the device.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t setCurrentFactor(uint8_t value);

  /**
   * @brief Saves the voltage and current calibration factors to the device.
   *
   * Persists the currently set voltage and current calibration factors to the device’s non-volatile memory.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t saveVoltageCurrentFactor(void);

  /**
   * @brief Initiates a jump to the device’s bootloader mode.
   *
   * Sends a command to the device to enter bootloader mode for firmware updates.
   *
   * @param[in] None
   *
   * @attention This operation may disrupt normal device operation until reset.
   *
   * @return
   *  - `UNIT_AC_MEASURE_OK`: Success
   */
  ac_measure_error_t jumpBootloader(void);

  /**
   * @brief Sets the I2C address of the device.
   *
   * Updates the device’s I2C address and stores it for subsequent communications.
   *
   * @param[in] addr The new I2C address to set.
   *
   * @attention Ensure the address is valid and does not conflict with other devices on the bus.
   *
   * @return uint8_t The newly set I2C address.
   */
  uint8_t setI2CAddress(uint8_t addr);

  /**
   * @brief Retrieves the current I2C address of the device.
   *
   * Reads the current I2C address from the device’s I2C address register.
   *
   * @param[in] None
   *
   * @attention Requires successful initialization via `begin()`.
   *
   * @return uint8_t The current I2C address.
   */
  uint8_t getI2CAddress(void);
};

#endif // AC_MEASURE_H

/* End of file -------------------------------------------------------- */