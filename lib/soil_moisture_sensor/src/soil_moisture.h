/**
 * @file       soil_moisture.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for soil moisture sensor library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SOIL_MOISTURE_H
  #define SOIL_MOISTURE_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class SoilMoisture
{
public:
  /**
   * @brief  init the Soil Moisture Sensor with a specified pin
   */
  SoilMoisture(int pin);

  /**
   * @brief  Read the analog value from the sensor using the arduino function analogRead
   *
   * @param[in]  void
   *
   * @return     uint32_t            The analog value read from the sensor.
   *
   */
  void read();

  /**
   * @brief Read the sensor and map its raw analog values to a range (e.g., 0-100).
   *
   * @param[in]   minValue      Minimum value of the range
   * @param[in]   maxValue      Maximum value of the range
   *
   * @return      uint32_t           The calibrated sensor value mapped to a range of minValue to maxValue.
   */
  uint32_t readAndMap(int minValue, int maxValue);

  /**
   * @brief Check if the moisture level is above or below a threshold.
   *
   * @param[in] threshold   Threshold value for moisture.
   *
   * @return bool           True if moisture is above the threshold, false otherwise.
   */
  bool isAboveThreshold(int threshold);

  /**
   * @brief Get the analog sensor value
   *
   * @param[in]   None
   *
   * @return      int           The analog sensor value
   */
  int getMoisture();

  /**
   * @brief Get the percentage sensor value from the analog value
   *
   * @param[in]   None
   *
   * @return      int           The percentage value
   */
  int getMoisturePercentage();

  /**
   * @brief Get the average sensor reading over a specified number of samples.
   *
   * @param[in] samples   Number of samples to average.
   *
   * @return int          The averaged sensor value.
   */
  int getAverageReading(int samples = 10);

  /**
   * @brief Set a callback function to trigger when moisture crosses a threshold.
   *
   * @param[in] threshold Threshold for triggering the callback.
   * @param[in] callback  Function to call when threshold is crossed.
   */
  void onThresholdCross(int threshold, void (*callback)());

private:
  int      _pin;                 /** SIG pin of the Soil Moisture Sensor */
  uint32_t sensorValue[2];       /** Value from the sensor */
  void (*_callback)() = nullptr; /** Callback function */
};

#endif // SOIL_MOISTURE_H

/* End of file -------------------------------------------------------- */