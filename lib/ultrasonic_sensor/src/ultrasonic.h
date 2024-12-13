/**
 * @file       ultrasonic.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header file for Ultrasonic sensor library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef ULTRASONIC_H
  #define ULTRASONIC_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define ULTRASONIC_LIB_VERSION (F("0.1.0"))

  #define CM_DIVISOR             58
  #define MM_DIVISOR             5.8
  #define IN_DIVISOR             148

  #define CM_INDEX               0
  #define MM_INDEX               1
  #define IN_INDEX               2
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  ULTRASONIC_OK,        /* No error */
  ULTRASONIC_ERR,       /* Generic error */
  ULTRASONIC_INIT_ERR,  /* Initialization error */
  ULTRASONIC_RESET_ERR, /* Reset error */
  ULTRASONIC_TIMEOUT,   /* Timeout error*/
} ultrasonic_error_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Ultrasonic sensor class for measuring distances.
 *
 * This class provides functionality to interact with an ultrasonic sensor, measure the time duration of
 * the echo signal, and calculate distances in different units.
 */
class Ultrasonic
{
private:
  uint8_t       triggerPin;
  uint8_t       echoPin;
  unsigned int  distance[3];
  unsigned long timeout;

  /**
   * @brief  Measures the time duration of the echo received by the ultrasonic sensor.
   *
   * This function resets the trigger pin, sends a pulse to initiate measurement, and reads the duration
   * of the echo signal received, which corresponds to the distance measured by the ultrasonic sensor.
   *
   * @param[in]     None
   * @param[out]    None
   * @param[inout]  None
   *
   * @attention  Ensure that the trigger pin and echo pin are correctly connected and configured.
   *             This function may block for a short period while waiting for the echo signal.
   *
   * @return
   *  - The duration of the echo signal in microseconds.
   */
  unsigned int duration();

public:
  /**
   * @brief  Initializes the Ultrasonic sensor with the specified trigger and echo pins, and sets the timeout
   * value.
   *
   * This constructor sets up the ultrasonic sensor by assigning the trigger and echo pins
   * and configuring them as output and input respectively. It also sets the timeout for the pulse
   * measurement.
   *
   * @param[in]     _triggerPin  The pin number connected to the trigger pin of the ultrasonic sensor.
   * @param[in]     _echoPin     The pin number connected to the echo pin of the ultrasonic sensor.
   * @param[in]     timeOut      The timeout value for the pulse measurement in microseconds.
   * @param[out]    None
   * @param[inout]  None
   *
   * @attention  Ensure that the pin numbers passed are correctly connected to the ultrasonic sensor.
   *             The timeout should be set according to the maximum expected distance measurement, default is
   * 23000 microseconds.
   *
   */
  Ultrasonic(uint8_t _triggerPin, uint8_t _echoPin, unsigned long timeOut = 20000UL);

  /**
   * @brief  Reads the distance measured by the ultrasonic sensor in the specified unit.
   *
   * This function reads the time duration from the ultrasonic sensor and converts it
   * to a distance value in centimeters, millimeters, or inches based on the specified unit.
   *
   * @param[in]     unit  The unit in which the distance is to be measured:
   *                      - 'c': Centimeters
   *                      - 'm': Millimeters
   *                      - 'i': Inches
   * @param[out]    None
   * @param[inout]  None
   *
   * @attention  Ensure that the ultrasonic sensor is properly connected and functioning.
   *
   * @return
   *  - The distance measured by the sensor in the specified unit.
   *
   *  - -1: If an invalid unit is provided.
   */
  void read(char unit);

  unsigned int getDistance(char unit);

  /**
   * @brief  Sets the timeout value for the ultrasonic sensor's pulse measurement.
   *
   * This inline function allows you to configure the timeout value, which determines the
   * maximum duration the sensor will wait for the echo signal before timing out.
   *
   * @param[in]     timeOut  The timeout value in microseconds to be set.
   * @param[out]    None
   * @param[inout]  None
   *
   * @attention  Ensure that the timeout value is appropriate for the maximum distance you expect to measure.
   *             Setting a very high timeout value can cause delays in the sensor's responsiveness.
   *
   * @return
   *  void
   */
  inline void setTimeout(unsigned long timeOut) { timeout = timeOut; }
};

#endif // ULTRASONIC_H

/* End of file -------------------------------------------------------- */