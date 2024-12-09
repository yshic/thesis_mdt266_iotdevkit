/**
 * @file       mini_fan.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for Mini Fan Module library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef MINI_FAN_H
#define MINI_FAN_H

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
 * @brief  Mini Fan control class for managing fan operations.
 *
 * This class provides methods to control a mini fan module, including setting the fan speed,
 * toggling the fan state, checking if the fan is running, and setting the fan speed as a percentage.
 */
class MiniFan
{
public:
  /**
   * @brief  Initializes the Mini Fan Module with a specified pin.
   *
   * @param[in]  pin  The pin number connected to the fan's control signal.
   */
  MiniFan(int pin);

  /**
   * @brief  Sets the fan speed using a PWM value.
   *
   * This function sets the speed of the fan by writing a PWM value to the control pin.
   *
   * @param[in]  speed  The speed of the fan (0-255).
   */
  void setFanSpeed(int speed);

  /**
   * @brief  Sets the fan speed as a percentage.
   *
   * This function sets the speed of the fan by converting a percentage value to a PWM value and writing it to
   * the control pin.
   *
   * @param[in]  percentage  The speed of the fan as a percentage (0-100%).
   */
  void setFanSpeedPercentage(int percentage);

  /**
   * @brief  Gets the current speed of the fan.
   *
   * This function returns the current speed of the fan as a PWM value.
   *
   * @return  int  The current speed of the fan (0-255).
   */
  int getFanSpeed();

  /**
   * @brief  Gets the current speed of the fan as a percentage.
   *
   * This function returns the current speed of the fan, converted from the PWM value to a percentage
   * (0-100%).
   *
   * @return  int  The current speed of the fan as a percentage (0-100%).
   */
  int getFanSpeedPercentage();

  /**
   * @brief  Toggles the fan state between ON and OFF.
   *
   * This function toggles the state of the fan. If the fan is currently ON, it will be turned OFF, and vice
   * versa.
   */
  void toggleFan();

  /**
   * @brief  Checks if the fan is currently running.
   *
   * @return  true if the fan is running, false otherwise.
   */
  bool isFanRunning();

private:
  int  _pin;   /** SIG pin of the Mini Fan */
  int  _speed; /** Speed of the fan */
  bool status; /**< Status of the fan (ON/OFF) */
};

#endif // MINI_FAN_H

/* End of file -------------------------------------------------------- */