/**
 * @file       es_soil_7n1.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-13
 * @author     Tuan Nguyen
 *
 * @brief      Header file for ES_SOIL_7N1 library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef ES_SOIL_7N1_H
  #define ES_SOIL_7N1_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define ES_SOIL_7N1_LIB_VERSION (F("0.1.0"))

// #define DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

/* Public enumerate/structure ----------------------------------------- */

// Error code for ES Soil 7 in 1
typedef enum
{
  ES_SOIL_7N1_OK = 0,
  ES_SOIL_7N1_ERR,
  ES_SOIL_7N1_ERR_READ,
  ES_SOIL_7N1_ERR_WRITE,
  ES_SOIL_7N1_TIMEOUT
} es_soil_7n1_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages soil sensor measurements via RS485 Modbus, providing access to pH, moisture, temperature,
 * conductivity, and nutrient levels.
 *
 * The `EsSoil7n1` class facilitates interaction with a 7-in-1 soil sensor over an RS485 Modbus interface.
 * It supports reading soil parameters including pH, moisture, temperature, conductivity, nitrogen,
 * phosphorus, and potassium. The class uses Modbus commands to retrieve data and stores converted values for
 * easy access.
 *
 * ### Features:
 *
 * - Reads soil pH, moisture, temperature, conductivity, and NPK (nitrogen, phosphorus, potassium) levels.
 *
 * - Supports combined readings for efficiency (e.g., temperature and moisture, NPK).
 *
 * - Stores sensor values in an internal array for retrieval via getter methods.
 *
 * - Provides debug output for raw Modbus responses when enabled.
 *
 * ### Usage:
 *
 * Instantiate the class and initialize the RS485 serial interface. Call the appropriate `read` methods (e.g.,
 * `readSoilPh()`, `readSoilNPK()`) to retrieve sensor data, then use getter methods (e.g., `getSoilPh()`) to
 * access the converted values. Ensure the sensor is powered and connected properly before reading.
 *
 * ### Dependencies:
 *
 * - Requires an RS485 serial interface (`rs485Serial1`) with Modbus command support.
 *
 * - Sensor must be connected to a valid RS485 bus.
 *
 * - Optional debug output requires a Serial interface when `DEBUG_PRINT` or
 * `DEBUG_PRINT_ES_SOIL_RAW_RESPONSE` is defined.
 */
class EsSoil7n1
{
public:
  /* Read methods ------------------------------------------------------ */

  /**
   * @brief  Reads soil pH value.
   *
   * This function sends a specific command over RS485 to read the soil's pH level
   * and processes the sensor's response to extract and store the pH value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that the RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: pH reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilPh();

  /**
   * @brief  Reads soil moisture level.
   *
   * This function sends a specific command over RS485 to read the soil's moisture level
   * and processes the sensor's response to extract and store the moisture value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Moisture reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilMoisture();

  /**
   * @brief  Reads soil temperature.
   *
   * This function sends a specific command over RS485 to read the soil's temperature
   * and processes the sensor's response to extract and store the temperature value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Temperature reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilTemperature();

  /**
   * @brief  Reads soil electrical conductivity.
   *
   * This function sends a specific command over RS485 to read the soil's electrical conductivity
   * and processes the sensor's response to extract and store the conductivity value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Conductivity reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilConductivity();

  /**
   * @brief  Reads soil nitrogen level.
   *
   * This function sends a specific command over RS485 to read the soil's nitrogen level
   * and processes the sensor's response to extract and store the nitrogen value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Nitrogen reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilNitrogen();

  /**
   * @brief  Reads soil phosphorus level.
   *
   * This function sends a specific command over RS485 to read the soil's phosphorus level
   * and processes the sensor's response to extract and store the phosphorus value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Phosphorus reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilPhosphorus();

  /**
   * @brief  Reads soil potassium level.
   *
   * This function sends a specific command over RS485 to read the soil's potassium level
   * and processes the sensor's response to extract and store the potassium value.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Potassium reading was successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilPotassium();

  /**
   * @brief  Reads soil temperature and moisture values simultaneously.
   *
   * This function sends a specific command over RS485 to read both the soil's temperature
   * and moisture levels in a single operation. It processes the sensor's response to extract
   * and store these values.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: Temperature and moisture readings were successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilTempAndMoisture();

  /**
   * @brief  Reads soil nitrogen, phosphorus, and potassium (NPK) levels simultaneously.
   *
   * This function sends a specific command over RS485 to read the soil's nitrogen, phosphorus,
   * and potassium levels in a single operation. It processes the sensor's response to extract
   * and store these values.
   *
   * @param[in]     None
   *
   * @attention  Ensure that RS485 communication and sensor connections are functional
   *             before calling this function.
   *
   * @return
   *  - `ES_SOIL_7N1_OK`: NPK readings were successful.
   *
   *  - `ES_SOIL_7N1_ERR`: Error occurred during communication or response validation.
   */
  es_soil_7n1_error_t readSoilNPK();

  /* Getter methods ------------------------------------------------------ */

  /**
   * @brief Retrieves the soil pH value.
   *
   * Returns the previously read soil pH value stored in the internal sensor array,
   * converted to a floating-point representation.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilPh()`.
   *
   * @return float The soil pH value.
   */
  float getSoilPh();

  /**
   * @brief Retrieves the soil moisture level.
   *
   * Returns the previously read soil moisture value stored in the internal sensor array,
   * converted to a floating-point representation in percentage relative humidity (% RH).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilMoisture()` or `readSoilTempAndMoisture()`.
   *
   * @return float The soil moisture value in % RH.
   */
  float getSoilMoisture();

  /**
   * @brief Retrieves the soil temperature.
   *
   * Returns the previously read soil temperature value stored in the internal sensor array,
   * converted to a floating-point representation in degrees Celsius.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilTemperature()` or `readSoilTempAndMoisture()`.
   *
   * @return float The soil temperature value in °C.
   */
  float getSoilTemperature();

  /**
   * @brief Retrieves the soil conductivity.
   *
   * Returns the previously read soil conductivity value stored in the internal sensor array,
   * represented as a floating-point value in microsiemens per centimeter (µS/cm).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilConductivity()`.
   *
   * @return float The soil conductivity value in µS/cm.
   */
  float getSoilConductivity();

  /**
   * @brief Retrieves the soil nitrogen level.
   *
   * Returns the previously read soil nitrogen value stored in the internal sensor array,
   * represented as a floating-point value in milligrams per kilogram (mg/kg).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilNitrogen()` or `readSoilNPK()`.
   *
   * @return float The soil nitrogen value in mg/kg.
   */
  float getSoilNitrogen();

  /**
   * @brief Retrieves the soil phosphorus level.
   *
   * Returns the previously read soil phosphorus value stored in the internal sensor array,
   * represented as a floating-point value in milligrams per kilogram (mg/kg).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilPhosphorus()` or `readSoilNPK()`.
   *
   * @return float The soil phosphorus value in mg/kg.
   */
  float getSoilPhosphorus();

  /**
   * @brief Retrieves the soil potassium level.
   *
   * Returns the previously read soil potassium value stored in the internal sensor array,
   * represented as a floating-point value in milligrams per kilogram (mg/kg).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful call to `readSoilPotassium()` or `readSoilNPK()`.
   *
   * @return float The soil potassium value in mg/kg.
   */
  float getSoilPotassium();

private:
  uint8_t address         = 0x01;
  float   sensorValues[7] = {0.0}; // index 0 - pH
                                   // index 1 - Moisture
                                   // index 2 - Temperature
                                   // index 3 - Conductivity
                                   // index 4 - Nitrogen
                                   // index 5 - Phosphorus
                                   // index 6 - Potassium
};

#endif // ES_SOIL_7N1_H

/* End of file -------------------------------------------------------- */