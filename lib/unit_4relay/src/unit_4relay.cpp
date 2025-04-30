/**
 * @file       unit_4relay.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-28
 * @author     Tuan Nguyen
 *
 * @brief      Source file for unit_4relay.cpp
 *
 */

/* Includes ----------------------------------------------------------- */
#include "unit_4relay.h"
#include "bsp_i2c.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */
unit_4relay_error_t Unit4Relay::begin()
{
  if (!bspI2CExist(UNIT_4RELAY_I2C_ADDR))
  {
    return UNIT_4RELAY_ERR_I2C;
  }

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::init(bool mode)
{
  if (bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_REG, mode) != BSP_I2C_OK)
  {
    return UNIT_4RELAY_ERR_INIT;
  }

  if (bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, 0) != BSP_I2C_OK)
  {
    return UNIT_4RELAY_ERR_INIT;
  }

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::relayWrite(uint8_t number, bool state)
{
  if (number >= UNIT_4RELAY_MAX_RELAYS)
  {
    return UNIT_4RELAY_ERR_INDEX;
  }

  uint8_t stateFromDevice;

  bspI2CReadByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, stateFromDevice);

  if (state == 0)
  {
    stateFromDevice &= ~(0x01 << number);
  }
  else
  {
    stateFromDevice |= (0x01 << number);
  }

  relayState[number] = state;

  bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, stateFromDevice);

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::relayAll(bool state)
{
  bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, state * (0x0f));

  for (int i = 0; i < UNIT_4RELAY_MAX_RELAYS; i++)
  {
    relayState[i] = state;
  }

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::ledWrite(uint8_t number, bool state)
{
  if (number >= UNIT_4RELAY_MAX_RELAYS)
  {
    return UNIT_4RELAY_ERR_INDEX;
  }

  uint8_t stateFromDevice;

  bspI2CReadByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, stateFromDevice);
  if (state == 0)
  {
    stateFromDevice &= ~(UNIT_4RELAY_REG << number);
  }
  else
  {
    stateFromDevice |= (UNIT_4RELAY_REG << number);
  }

  ledState[number] = state;

  bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, stateFromDevice);

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::ledAll(bool state)
{
  bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_RELAY_REG, state * (0xf0));

  for (int i = 0; i < UNIT_4RELAY_MAX_RELAYS; i++)
  {
    ledState[i] = state;
  }

  return UNIT_4RELAY_OK;
}

unit_4relay_error_t Unit4Relay::switchMode(bool mode)
{
  bspI2CWriteByte(UNIT_4RELAY_I2C_ADDR, UNIT_4RELAY_REG, mode);

  return UNIT_4RELAY_OK;
}

int Unit4Relay::getRelayState(uint8_t number)
{
  if (number >= UNIT_4RELAY_MAX_RELAYS)
  {
    return UNIT_4RELAY_ERR_INDEX;
  }

  return relayState[number];
}

void Unit4Relay::setRelayState(bool state[4])
{
  for (int i = 0; i < UNIT_4RELAY_MAX_RELAYS; i++)
  {
    if (state[i])
    {
      relayState[i] = state[i];
    }
  }
}

int Unit4Relay::getLedState(uint8_t number)
{
  if (number >= UNIT_4RELAY_MAX_RELAYS)
  {
    return UNIT_4RELAY_ERR_INDEX;
  }

  return ledState[number];
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */