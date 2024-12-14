/**
 * @file       pir_sensor.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for PIR Sensor Library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef PIR_SENSOR_H
  #define PIR_SENSOR_H

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

/**
 * @brief  PIR Sensor control class for managing motion detection.
 *
 * This class provides methods to initialize and interact with a PIR sensor module, including basic and
 * advanced functions.
 */
class PIRSensor
{
public:
  /**
   * @brief  Initializes the PIR sensor with the specified pin.
   *
   * @param[in] pin The pin connected to the PIR sensor output.
   *
   * @return    None
   */
  PIRSensor(uint8_t pin);

  /**
   * @brief  Reads the current motion status from the PIR sensor.
   *
   * @param[in]   None
   *
   * @return      None
   *
   */
  void read();

  /**
   * @brief  Get the current motion status from the PIR sensor.
   *
   * @param[in]   None
   *
   * @return  uint8_t
   *  - 1: Motion detected
   *  - 0: No motion detected
   */
  uint8_t getStatus();

  /**
   * @brief  Sets a debounce time to avoid false motion triggers.
   *
   * @param[in] debounceTime  The debounce time in milliseconds.
   *
   * @attention  This function should be called before using `read`.
   *
   * @return
   *  - 0: Success
   *  - 1: Error
   */
  int setDebounceTime(unsigned long debounceTime);

  /**
   * @brief  Checks if motion is continuously detected for a specified duration.
   *
   * @param[in] duration The duration in milliseconds to check for continuous motion.
   *
   * @return
   *  - 1: Motion detected continuously for the duration
   *  - 0: No continuous motion detected
   */
  int isMotionContinuous(unsigned long duration);

  /**
   * @brief  Gets the last time motion was detected.
   *
   * @return  The timestamp (in milliseconds) of the last motion detection.
   */
  unsigned long getLastMotionTime();

  /**
   * @brief  Enables a callback function to be called on motion detection.
   *
   * @param[in] callback  The function to be called when motion is detected.
   *
   * @attention  The callback function must have the signature `void callback()`.
   *
   * @return
   *  - 0: Success
   *  - 1: Error
   */
  int setMotionCallback(void (*callback)());

  /**
   * @brief  Disables the motion detection callback.
   *
   * @return
   *  - 0: Success
   *  - 1: Error
   */
  int disableMotionCallback();

  /**
   * @brief  Sets the sensitivity of the PIR sensor.
   *
   * @param[in] level  The sensitivity level (e.g., 1: Low, 2: Medium, 3: High).
   *
   * @return
   *  - 0: Success
   *  - 1: Error
   */
  int setSensitivity(int level);

  /**
   * @brief  Resets the PIR sensor to default settings.
   *
   * @return
   *  - 0: Success
   *  - 1: Error
   */
  int reset();

private:
  uint8_t       _pin;            /**< Pin connected to the PIR sensor output. */
  uint8_t       status;          /** Status of the detection */
  unsigned long _lastMotionTime; /**< Timestamp of the last detected motion. */
  unsigned long _debounceTime;   /**< Debounce time in milliseconds. */
  void (*_motionCallback)();     /**< Callback function for motion detection. */
};

#endif // PIR_SENSOR_H

/* End of file -------------------------------------------------------- */