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

/**
 * @brief Enum for PIR sensor error codes.
 */
typedef enum
{
  PIR_SENSOR_OK = 0,   /* No error */
  PIR_SENSOR_ERR,      /* Generic error */
  PIR_SENSOR_ERR_INIT, /* Initialization error */
  PIR_SENSOR_TIMEOUT   /* Timeout error */
} pir_sensor_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages motion detection using a PIR sensor module.
 *
 * The `PIRSensor` class provides functionality for detecting motion via a Passive Infrared (PIR) sensor
 * connected to an Arduino pin. It supports reading motion status, setting debounce times, detecting
 * continuous motion, managing callbacks for motion events, and adjusting sensitivity.
 *
 * ### Features:
 *
 * - Reads motion status with debouncing to prevent false triggers.
 *
 * - Tracks the timestamp of the last motion detection.
 *
 * - Detects continuous motion over a specified duration.
 *
 * - Supports callback functions for motion detection events.
 *
 * - Allows sensitivity adjustment through debounce time settings.
 *
 * ### Usage:
 *
 * Instantiate the class with the appropriate digital input pin. Call `read()` periodically to update the
 * motion status. Use `getStatus()` to check for motion, `setDebounceTime()` or `setSensitivity()` to reduce
 * false positives, and `setMotionCallback()` for event-driven responses. Use `reset()` to restore default
 * settings.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with digital input support.
 *
 * - PIR sensor must be connected to a valid digital input pin.
 *
 * - Depends on `bsp_gpio.h` for GPIO operations (`bspGpioDigitalRead`, `bspGpioPinMode`).
 *
 * - Callback functions must be defined before use with `setMotionCallback()`.
 */
class PIRSensor
{
public:
  /**
   * @brief Constructor to initialize the PIR sensor with a specified pin.
   *
   * Configures the specified pin as a digital input for reading the PIR sensor output.
   *
   * @param[in] pin The digital pin connected to the PIR sensor output.
   *
   * @attention Ensure the pin is a valid digital input pin on the microcontroller.
   */
  PIRSensor(uint8_t pin);

  /**
   * @brief Reads the current motion status from the PIR sensor.
   *
   * Checks the sensor's digital output, applies debouncing, and updates the internal motion status. Triggers
   * the callback function if motion is detected and a callback is set.
   *
   * @param[in] None
   *
   * @attention Ensure `setDebounceTime()` is called if debouncing is needed to avoid false triggers.
   */
  void read();

  /**
   * @brief Retrieves the current motion status.
   *
   * Returns the latest motion status from the internal state.
   *
   * @param[in] None
   *
   * @attention Requires a prior call to `read()` to update the status.
   *
   * @return uint8_t
   *  - `1`: Motion detected
   *
   *  - `0`: No motion detected
   */
  uint8_t getStatus();

  /**
   * @brief Sets the debounce time to avoid false motion triggers.
   *
   * Configures the minimum time between consecutive motion detections to filter out noise.
   *
   * @param[in] debounceTime The debounce time in milliseconds.
   *
   * @attention Should be called before `read()` if debouncing is required.
   *
   * @return
   *  - `PIR_SENSOR_OK`: Success
   *
   *  - `PIR_SENSOR_ERR`: Invalid debounce time
   */
  pir_sensor_error_t setDebounceTime(unsigned long debounceTime);

  /**
   * @brief Checks if motion is continuously detected for a specified duration.
   *
   * Monitors the sensor output for the specified duration to confirm continuous motion.
   *
   * @param[in] duration The duration in milliseconds to check for continuous motion.
   *
   * @attention This function blocks until the duration expires or motion ceases.
   *
   * @return
   *  - `1`: Motion detected continuously for the duration
   *
   *  - `0`: No continuous motion detected
   */
  int isMotionContinuous(unsigned long duration);

  /**
   * @brief Retrieves the timestamp of the last motion detection.
   *
   * Returns the time (in milliseconds) when motion was last detected.
   *
   * @param[in] None
   *
   * @attention Requires a prior call to `read()` to update the motion timestamp.
   *
   * @return unsigned long The timestamp (in milliseconds) of the last motion detection.
   */
  unsigned long getLastMotionTime();

  /**
   * @brief Enables a callback function for motion detection events.
   *
   * Configures a callback function to be invoked when motion is detected during a `read()` call.
   *
   * @param[in] callback The function to call when motion is detected (signature: `void callback()`).
   *
   * @attention The callback must be defined and have no parameters or return value.
   *
   * @return
   *  - `PIR_SENSOR_OK`: Success
   *
   *  - `PIR_SENSOR_ERR`: Invalid callback
   */
  pir_sensor_error_t setMotionCallback(void (*callback)());

  /**
   * @brief Disables the motion detection callback.
   *
   * Clears the callback function, preventing it from being triggered on motion detection.
   *
   * @param[in] None
   *
   * @return
   *  - `PIR_SENSOR_OK`: Success
   */
  pir_sensor_error_t disableMotionCallback();

  /**
   * @brief Sets the sensitivity of the PIR sensor.
   *
   * Adjusts the debounce time to simulate sensitivity levels: low (1000ms), medium (500ms), or high (200ms).
   *
   * @param[in] level The sensitivity level (1: Low, 2: Medium, 3: High).
   *
   * @attention Higher sensitivity reduces debounce time, increasing responsiveness but potentially false
   * triggers.
   *
   * @return
   *  - `PIR_SENSOR_OK`: Success
   *
   *  - `PIR_SENSOR_ERR`: Invalid sensitivity level
   */
  pir_sensor_error_t setSensitivity(int level);

  /**
   * @brief Resets the PIR sensor to default settings.
   *
   * Clears the motion status, last motion timestamp, debounce time, and callback function.
   *
   * @param[in] None
   *
   * @return
   *  - `PIR_SENSOR_OK`: Success
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