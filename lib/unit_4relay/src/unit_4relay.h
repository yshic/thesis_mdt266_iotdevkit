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

/**
 * @brief Enum for Unit 4-Relay module error codes.
 */
typedef enum
{
  UNIT_4RELAY_OK = 0,    /* No error */
  UNIT_4RELAY_ERR,       /* Generic error */
  UNIT_4RELAY_ERR_INIT,  /* Initialization error */
  UNIT_4RELAY_ERR_INDEX, /* Index error */
  UNIT_4RELAY_ERR_I2C,   /* I2C error */
} unit_4relay_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief Manages a 4-relay module with I2C interface for relay and LED control.
 *
 * The `Unit4Relay` class provides functionality for controlling a 4-relay module over I2C, including
 * initializing the module, setting relay and LED states individually or collectively, switching operating
 * modes (synchronous/asynchronous), and retrieving relay and LED states. It maintains internal state arrays
 * for relays and LEDs.
 *
 * ### Features:
 *
 * - Controls up to four relays with ON/OFF states.
 *
 * - Manages indicator LEDs corresponding to each relay.
 *
 * - Supports synchronous and asynchronous operating modes.
 *
 * - Provides individual and collective control for relays and LEDs.
 *
 * - Retrieves current relay and LED states with error handling.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` to verify I2C connectivity, followed by `init()` to set the
 * operating mode and reset relays. Use `relayWrite()` or `relayAll()` to control relays, `ledWrite()` or
 * `ledAll()` for LEDs, and `switchMode()` to change modes. Retrieve states with `getRelayState()` or
 * `getLedState()`. Use `setRelayState()` to stage relay states without immediate I2C writes.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with I2C support.
 *
 * - Module must be connected to the I2C bus at address `UNIT_4RELAY_I2C_ADDR` (0x26).
 *
 * - Depends on `bsp_i2c.h` for I2C operations (`bspI2CExist`, `bspI2CWriteByte`, `bspI2CReadByte`).
 */
class Unit4Relay
{
public:
  /**
   * @brief Initializes the Unit 4-Relay module.
   *
   * Checks for the presence of the module on the I2C bus at address 0x26.
   *
   * @param[in] None
   *
   * @attention Ensure the module is powered and connected to the I2C bus before calling.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Module detected and ready
   *
   *  - `UNIT_4RELAY_ERR_I2C`: Module not found or I2C communication error
   */
  unit_4relay_error_t begin();

  /**
   * @brief Configures the operating mode and turns off all relays.
   *
   * Sets the module to synchronous or asynchronous mode and initializes all relays to OFF.
   *
   * @param[in] mode Operating mode:
   *
   *                 - `0`: Asynchronous mode
   *
   *                 - `1`: Synchronous mode
   *
   * @attention Call after `begin()` to ensure proper initialization.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   *
   *  - `UNIT_4RELAY_ERR_INIT`: Failed to write initial configuration
   */
  unit_4relay_error_t init(bool mode);

  /**
   * @brief Sets the state of an individual relay.
   *
   * Turns a specified relay ON or OFF by updating the I2C register and internal state.
   *
   * @param[in] number Relay index (0–3).
   * @param[in] state Desired state (`true` for ON, `false` for OFF).
   *
   * @attention Ensure the relay index is valid (0–3).
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   *
   *  - `UNIT_4RELAY_ERR_INDEX`: Invalid relay index
   */
  unit_4relay_error_t relayWrite(uint8_t number, bool state);

  /**
   * @brief Sets all relays to the same state.
   *
   * Turns all relays ON or OFF simultaneously and updates the internal state.
   *
   * @param[in] state Desired state for all relays (`true` for ON, `false` for OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   */
  unit_4relay_error_t relayAll(bool state);

  /**
   * @brief Sets the state of an individual LED.
   *
   * Turns a specified indicator LED ON or OFF by updating the I2C register and internal state.
   *
   * @param[in] number LED index (0–3).
   * @param[in] state Desired state (`true` for ON, `false` for OFF).
   *
   * @attention Ensure the LED index is valid (0–3).
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   *  - `UNIT_4RELAY_ERR_INDEX`: Invalid LED index
   */
  unit_4relay_error_t ledWrite(uint8_t number, bool state);

  /**
   * @brief Sets all LEDs to the same state.
   *
   * Turns all indicator LEDs ON or OFF simultaneously and updates the internal state.
   *
   * @param[in] state Desired state for all LEDs (`true` for ON, `false` for OFF).
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   */
  unit_4relay_error_t ledAll(bool state);

  /**
   * @brief Sets the operating mode of the relay module.
   *
   * Changes the module’s operating mode to synchronous or asynchronous.
   *
   * @param[in] mode Desired mode (`0` for asynchronous, `1` for synchronous).
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   */
  unit_4relay_error_t switchMode(bool mode);

  /**
   * @brief Retrieves the state of a specific relay.
   *
   * Returns the current state of the specified relay from the internal state array.
   *
   * @param[in] number Relay index (0–3).
   *
   * @attention Ensure the relay index is valid (0–3).
   *
   * @return
   *  - `0`: Relay is OFF
   *
   *  - `1`: Relay is ON
   *
   *  - `UNIT_4RELAY_ERR_INDEX`: Invalid relay index
   */
  int getRelayState(uint8_t number);

  /**
   * @brief Retrieves the state of a specific LED.
   *
   * Returns the current state of the specified LED from the internal state array.
   *
   * @param[in] number LED index (0–3).
   *
   * @attention Ensure the LED index is valid (0–3).
   *
   * @return
   *  - `0`: LED is OFF
   *
   *  - `1`: LED is ON
   *
   *  - `UNIT_4RELAY_ERR_INDEX`: Invalid LED index
   */
  int getLedState(uint8_t number);

  /**
   * @brief Stages relay states without writing to the module.
   *
   * Updates the internal relay state array with the provided states, deferring physical changes until
   * `applyRelayState()` is called.
   *
   * @param[in] state Array of four boolean values representing the desired state for each relay (`true` for
   * ON, `false` for OFF).
   *
   * @attention The state array must contain exactly four elements. Use `applyRelayState()` to write the
   * staged states to the module.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   */
  unit_4relay_error_t setRelayStates(bool state[4]);

  /**
   * @brief Applies the staged relay states to the module.
   *
   * Writes the internal relay state array to the module’s I2C register, turning relays ON or OFF as specified
   * by the states set via `setRelayState()`.
   *
   * @param[in] None
   *
   * @attention Requires a prior call to `setRelayState()` to stage the desired relay states. Ensure the
   * module is initialized with `begin()` and `init()`.
   *
   * @return
   *  - `UNIT_4RELAY_OK`: Success
   *
   *  - `UNIT_4RELAY_ERR_I2C`: I2C communication error
   */
  unit_4relay_error_t applyRelayState();

private:
  bool relayState[4] = {false};
  bool ledState[4]   = {false};
};

#endif // UNIT_4RELAY_H

/* End of file -------------------------------------------------------- */