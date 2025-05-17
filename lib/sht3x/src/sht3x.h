/**
 * @file       sht3x.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-21
 * @author     Tuan Nguyen
 *
 * @brief      Header file for SHT3X library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SHT3X_H
  #define SHT3X_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define SHT3X_LIB_VERSION       (F("0.1.0"))

  #define SHT3X_I2C_ADDR_DEFAULT  0x44
  #define SHT3X_HUMIDITY_INDEX    0
  #define SHT3X_TEMPERATURE_INDEX 1

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  SHT3X_OK = 0,      /* No error */
  SHT3X_ERR,         /* Generic error */
  SHT3X_ERR_INIT,    /* Initialization error */
  SHT3X_ERR_RESET,   /* Reset error */
  SHT3X_TIMEOUT,     /* Timeout error*/
  SHT3X_ERR_I2C,     /* I2C error */
  SHT3X_ERR_CHECKSUM /* Checksum error */
} sht3x_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages temperature and humidity measurements using the SHT3X sensor.
 *
 * The `SHT3X` class provides functionality for interfacing with the SHT3X temperature and humidity sensor
 * over an I2C interface. It supports initializing the sensor, updating temperature and humidity readings, and
 * retrieving values in Celsius, Fahrenheit, or percentage humidity. The class stores sensor data internally
 * for efficient access.
 *
 * ### Features:
 *
 * - Reads temperature and humidity with high repeatability.
 *
 * - Supports configurable I2C address (default: 0x44).
 *
 * - Provides temperature in both Celsius and Fahrenheit.
 *
 * - Stores humidity as a percentage (0–100%).
 *
 * - Handles I2C communication with error checking.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` with the desired I2C address (default: 0x44) to initialize the
 * sensor. Use `update()` to refresh temperature and humidity readings, then retrieve values with
 * `getTemperatureCelcius()`, `getTemperatureFahrenheit()`, or `getHumidity()`. Ensure the sensor is connected
 * to the I2C bus before use.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with I2C support.
 *
 * - Sensor must be connected to a valid I2C bus at the specified address.
 *
 * - Depends on `bsp_i2c.h` for I2C operations (`bspI2CExist`, `bspI2CWriteByte`, `bspI2CReadBytes`).
 *
 * - Requires `config.h` for configuration settings.
 */
class SHT3X
{
public:
  /**
   * @brief Initializes the SHT3X sensor.
   *
   * Configures the sensor with the specified I2C address and checks for its presence on the I2C bus.
   *
   * @param[in] i2c_addr The I2C address of the sensor (default: `SHT3X_I2C_ADDR_DEFAULT`, 0x44).
   *
   * @attention Ensure the sensor is powered and connected to the I2C bus before calling.
   *
   * @return
   *  - `SHT3X_OK`: Sensor detected and initialized successfully
   *
   *  - `SHT3X_ERR_I2C`: Sensor not found or I2C communication error
   */
  sht3x_error_t begin(uint8_t i2c_addr = SHT3X_I2C_ADDR_DEFAULT);

  /**
   * @brief Updates temperature and humidity readings from the SHT3X sensor.
   *
   * Sends a high-repeatability measurement command with clock stretching enabled, reads the raw data, and
   * converts it to temperature (Celsius) and humidity (percentage) values, storing them internally.
   *
   * @param[in] None
   *
   * @attention Ensure the sensor is initialized with `begin()` and connected to the I2C bus. The function
   * includes delays (200ms + 50ms) for measurement completion.
   *
   * @return
   *  - `SHT3X_OK`: Measurement successful
   */
  sht3x_error_t update(void);

  /**
   * @brief Retrieves the last measured humidity.
   *
   * Returns the most recent humidity reading stored internally, as a percentage.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `update()`.
   *
   * @return float The relative humidity percentage (%RH).
   */
  float getHumidity();

  /**
   * @brief Retrieves the last measured temperature in Celsius.
   *
   * Returns the most recent temperature reading stored internally, in degrees Celsius.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `update()`.
   *
   * @return float The temperature in degrees Celsius.
   */
  float getTemperatureCelcius();

  /**
   * @brief Retrieves the last measured temperature in Fahrenheit.
   *
   * Converts the most recent temperature reading stored internally from Celsius to Fahrenheit.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `update()`.
   *
   * @return float The temperature in degrees Fahrenheit.
   */
  float getTemperatureFahrenheit();

private:
  float sensorValues[2] = {0}; // index 0 : Humidity
                               // index 1 : Temperature
  uint8_t _addr;
};

#endif // SHT3X_H

/* End of file -------------------------------------------------------- */