/**
 * @file       relay.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for Relay Module Library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef RELAY_H
  #define RELAY_H

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
class Relay
{
public:
  /**
   * @brief  Constructor for the Relay class.
   *
   * @param[in]     pin  Pin number connected to the relay.
   *
   * @attention  Sets the relay pin to OUTPUT mode and initializes the relay status to false.
   *
   * @return      None
   */
  Relay(int pin);

  /**
   * @brief  Turns the relay on.
   *
   * @param[in]     None
   *
   * @attention  Sets the relay status to true and writes HIGH to the relay pin.
   *
   * @return     None
   *
   */
  void on();

  /**
   * @brief  Turns the relay off.
   *
   * @param[in]  None
   *
   * @attention  Sets the relay status to false and writes LOW to the relay pin.
   *
   * @return     None
   *
   */
  void off();

  /**
   * @brief  Toggles the relay state.
   *
   * @param[in]  None
   *
   * @attention  Checks the current status and toggles the relay state accordingly.
   *
   * @return     None
   *
   */
  void toggle();

  /**
   * @brief  Gets the current status of the relay.
   *
   * @param[in]  None
   *
   * @attention  Returns the current status of the relay (true for on, false for off).
   *
   * @return
   *  - true: Relay is on
   *  - false: Relay is off
   */
  bool getStatus();

private:
  uint8_t _pin;
  bool    status;
};

#endif // RELAY_H

/* End of file -------------------------------------------------------- */