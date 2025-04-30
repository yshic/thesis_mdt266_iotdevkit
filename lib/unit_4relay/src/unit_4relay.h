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
   *
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
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   *
   *  - `UNIT_4RELAY_ERR_INIT(8210)`: Failed to write initial configuration to device.
   */
  unit_4relay_error_t init(bool mode);

  /**
   * @brief Sets the state of an individual relay.
   *
   * This function turns ON or OFF a specified relay by index.
   *
   * @param[in] number  Relay number (0-3).
   * @param[in] state   Desired relay state (`true` = ON, `false` = OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   *
   *  - `UNIT_4RELAY_ERR_INDEX (8211)`: Invalid relay index.
   */
  unit_4relay_error_t relayWrite(uint8_t number, bool state);

  /**
   * @brief Sets all relays to the same state.
   *
   * This function turns all relays ON or OFF.
   *
   * @param[in] state  Desired relay state for all (`true` = ON, `false` = OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   */
  unit_4relay_error_t relayAll(bool state);

  /**
   * @brief Sets the state of an individual LED.
   *
   * This function turns ON or OFF a specified indicator LED by index.
   *
   * @param[in] number  LED number (0-3).
   * @param[in] state   Desired LED state (`true` = ON, `false` = OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   *
   *  - `UNIT_4RELAY_ERR_INDEX (8211)`: Invalid LED index.
   */
  unit_4relay_error_t ledWrite(uint8_t number, bool state);

  /**
   * @brief Sets all LEDs to the same state.
   *
   * This function turns all indicator LEDs ON or OFF.
   *
   * @param[in] state  Desired LED state for all (`true` = ON, `false` = OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   */
  unit_4relay_error_t ledAll(bool state);

  /**
   * @brief Sets the operating mode of the relay unit.
   *
   * This function changes the internal mode of the device, such as LED or relay mode.
   *
   * @param[in] mode  Desired mode setting.
   *
   * @return
   *  - `UNIT_4RELAY_OK (8208)`: Success.
   */
  unit_4relay_error_t switchMode(bool mode);

  /**
   * @brief Gets the state of a specific relay.
   *
   * @param[in] number  Relay number (0-3).
   *
   * @return
   *  - Relay state (`0` = OFF, `1` = ON).
   *
   *  - `UNIT_4RELAY_ERR_INDEX (8211)`: If an invalid index is provided.
   */
  int getRelayState(uint8_t number);

  /**
   * @brief Gets the state of a specific LED.
   *
   * @param[in] number  LED number (0-3).
   *
   * @return
   *  - LED state (`0` = OFF, `1` = ON).
   *
   *  - `UNIT_4RELAY_ERR_INDEX (8211)`: If an invalid index is provided.
   */
  int getLedState(uint8_t number);

  void setRelayState(bool state[4]);

private:
  bool relayState[4] = {false};
  bool ledState[4]   = {false};
};

#endif // UNIT_4RELAY_H

/* End of file -------------------------------------------------------- */