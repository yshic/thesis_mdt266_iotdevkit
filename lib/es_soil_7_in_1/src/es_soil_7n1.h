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
class EsSoil7n1
{
public:
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

  float getSoilPh();
  float getSoilMoisture();
  float getSoilTemperature();
  float getSoilConductivity();
  float getSoilNitrogen();
  float getSoilPhosphorus();
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