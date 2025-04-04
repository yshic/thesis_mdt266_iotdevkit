/**
 * @file       sht4x.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-21
 * @author     Tuan Nguyen
 *
 * @brief      Header file for SHT4X library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SHT4X_H
  #define SHT4X_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define SHT4X_LIB_VERSION              (F("0.1.0"))

  #define SHT40_I2C_ADDR_44              0x44
  #define SHT40_I2C_ADDR_45              0x45
  #define SHT41_I2C_ADDR_44              0x44
  #define SHT41_I2C_ADDR_45              0x45
  #define SHT45_I2C_ADDR_44              0x44
  #define SHT45_I2C_ADDR_45              0x45

  #define SHT4X_HUMIDITY_INDEX           0
  #define SHT4X_TEMPERATURE_INDEX        1

  #define SHT4x_CMD_NOHEAT_HIGHPRECISION 0xFD /**< High precision measurement, no heater */
  #define SHT4x_CMD_NOHEAT_MEDPRECISION  0xF6 /**< Medium precision measurement, no heater */
  #define SHT4x_CMD_NOHEAT_LOWPRECISION  0xE0 /**< Low precision measurement, no heater */

  #define SHT4x_CMD_HIGHHEAT_1S          0x39 /**< High precision measurement, high heat for 1 sec */
  #define SHT4x_CMD_HIGHHEAT_100MS       0x32 /**< High precision measurement, high heat for 0.1 sec */
  #define SHT4x_CMD_MEDHEAT_1S           0x2F /**< High precision measurement, med heat for 1 sec */
  #define SHT4x_CMD_MEDHEAT_100MS        0x24 /**< High precision measurement, med heat for 0.1 sec */
  #define SHT4x_CMD_LOWHEAT_1S           0x1E /**< High precision measurement, low heat for 1 sec */
  #define SHT4x_CMD_LOWHEAT_100MS        0x15 /**< High precision measurement, low heat for 0.1 sec */

  #define SHT4x_READSERIAL               0x89 /**< Read Out of Serial Register */
  #define SHT4x_SOFTRESET                0x94 /**< Soft Reset */

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  SHT4X_OK = 0,      /* No error */
  SHT4X_ERR,         /* Generic error */
  SHT4X_ERR_INIT,    /* Initialization error */
  SHT4X_ERR_RESET,   /* Reset error */
  SHT4X_TIMEOUT,     /* Timeout error*/
  SHT4X_ERR_I2C,     /* I2C error */
  SHT4X_ERR_CHECKSUM /* Checksum error */
} sht4x_error_t;

typedef enum
{
  SHT4X_HIGH_PRECISION,
  SHT4X_MED_PRECISION,
  SHT4X_LOW_PRECISION,
} sht4x_precision_t;

/** Optional pre-heater configuration setting */
typedef enum
{
  SHT4X_NO_HEATER,
  SHT4X_HIGH_HEATER_1S,
  SHT4X_HIGH_HEATER_100MS,
  SHT4X_MED_HEATER_1S,
  SHT4X_MED_HEATER_100MS,
  SHT4X_LOW_HEATER_1S,
  SHT4X_LOW_HEATER_100MS,
} sht4x_heater_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief  Class for interfacing with the SHT4X temperature and humidity sensor.
 *
 * The SHT4X class provides methods to initialize, configure, and retrieve
 * temperature and humidity data from the SHT4X sensor over an I2C interface.
 */
class SHT4X
{
public:
  /**
   * @brief  Initializes the SHT4X sensor.
   *
   * This function checks if the SHT4X sensor is available on the I2C bus
   * and returns an appropriate status.
   *
   * @return
   *  - `SHT4X_OK`: Sensor detected and initialized successfully.
   *
   *  - `SHT4X_ERR_I2C`: Sensor not found or I2C communication error.
   */
  sht4x_error_t begin();

  /**
   * @brief  Updates sensor readings from the SHT4X.
   *
   * This function sends a measurement command to the SHT4X sensor,
   * waits for the required conversion time, reads the data,
   * verifies the checksum, and updates temperature and humidity values.
   *
   * @attention Ensure that the sensor is correctly connected to the I2C bus
   * and that appropriate delays are provided for accurate readings.
   *
   * @return
   *  - `SHT4X_OK`: Measurement successful.
   *
   *  - `SHT4X_ERR_CHECKSUM`: Data corruption detected due to checksum mismatch.
   */
  sht4x_error_t update(void);

  /**
   * @brief  Sets the measurement precision of the SHT4X sensor.
   *
   * This function allows changing the precision level of the sensor's measurements.
   *
   * @param[in]     prec Desired precision setting.
   */
  void setPrecision(sht4x_precision_t prec);

  /**
   * @brief  Gets the current measurement precision setting.
   *
   * @return The current precision setting of the sensor.
   */
  sht4x_precision_t getPrecision(void);

  /**
   * @brief  Sets the heater mode of the SHT4X sensor.
   *
   * This function enables or disables the built-in heater and adjusts its power level.
   *
   * @param[in]     heat Desired heater mode.
   */
  void setHeater(sht4x_heater_t heat);

  /**
   * @brief  Gets the current heater mode of the SHT4X sensor.
   *
   * @return The current heater mode setting.
   */
  sht4x_heater_t getHeater(void);

  /**
   * @brief  Retrieves the last measured temperature.
   *
   * This function returns the most recently updated temperature reading.
   *
   * @return The temperature in degrees Celsius.
   */
  float getTemperature();

  /**
   * @brief  Retrieves the last measured humidity.
   *
   * This function returns the most recently updated relative humidity reading.
   *
   * @return The relative humidity percentage (%RH).
   */
  float getHumidity();

private:
  float sensorValue[2] = {0.f}; // Humidity: index 0
                                // Temperature: index 1
  sht4x_precision_t _precision = SHT4X_HIGH_PRECISION;
  sht4x_heater_t    _heater    = SHT4X_NO_HEATER;
};

#endif // SHT40_h

/* End of file -------------------------------------------------------- */