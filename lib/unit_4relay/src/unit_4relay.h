/**
 * @file       unit_4relay.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-28
 * @author     Tuan Nguyen
 *
 * @brief      Header file for UNIT_4RELAY library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef UNIT_4RELAY_H
  #define UNIT_4RELAY_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define UNIT_4RELAY_LIB_VERSION (F("0.1.0"))

  #define UNIT_4RELAY_I2C_ADDR    0x26
  #define UNIT_4RELAY_REG         0x10
  #define UNIT_4RELAY_RELAY_REG   0x11

  #define UNIT_4RELAY_MAX_RELAYS  4
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  UNIT_4RELAY_OK = 0x2010, /* No error */
  UNIT_4RELAY_ERR,         /* Generic error */
  UNIT_4RELAY_ERR_INIT,    /* Initialization error */
  UNIT_4RELAY_ERR_INDEX,   /* Index error */
  UNIT_4RELAY_ERR_I2C,     /* I2C error */
} unit_4relay_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class Unit4Relay
{
public:
  /**
   * @brief  Initializes the Unit 4-Relay module.
   *
   * This function checks if the device is available on the I2C bus.
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Device detected and ready.
   *  - `UNIT_4RELAY_ERR_I2C (8213)`: Device not found.
   */
  unit_4relay_error_t begin();

  /**
   * @brief  Sets the mode of the device and turns off all relays.
   *
   * @param[in] mode Operation mode:
   *                 - `0`: Asynchronous mode.
   *                 - `1`: Synchronous mode.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t init(bool mode);

  /**
   * @brief  Controls the on/off state of a specific relay.
   *
   * @param[in] number Relay index (0 to 3).
   * @param[in] state  Relay state:
   *                   - `0`: OFF.
   *                   - `1`: ON.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t relayWrite(uint8_t number, bool state);

  /**
   * @brief  Sets the state of all relays at the same time.
   *
   * @param[in] state Relay state:
   *                  - `0`: All relays OFF.
   *                  - `1`: All relays ON.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t relayAll(bool state);

  /**
   * @brief  Controls the on/off state of a specific LED.
   *
   * @param[in] number LED index (0 to 3).
   * @param[in] state  LED state:
   *                   - `0`: OFF.
   *                   - `1`: ON.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t ledWrite(uint8_t number, bool state);

  /**
   * @brief  Sets the state of all LEDs at the same time.
   *
   * @param[in] state LED state:
   *                  - `0`: All LEDs OFF.
   *                  - `1`: All LEDs ON.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t ledAll(bool state);

  /**
   * @brief  Sets the operation mode of the device.
   *
   * @param[in] mode Operation mode:
   *                 - `0`: Asynchronous mode.
   *                 - `1`: Synchronous mode.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success.
   */
  unit_4relay_error_t switchMode(bool mode);

  int getRelayState(uint8_t number);

  int getLedState(uint8_t number);

private:
  bool relayState[4] = {false};
  bool ledState[4]   = {false};
};

#endif // UNIT_4RELAY_H

/* End of file -------------------------------------------------------- */