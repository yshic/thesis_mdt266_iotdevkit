/**
 * @file       bsp_i2c.cpp
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_i2c
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include <Wire.h>
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
static TwoWire *i2cWire = &Wire;

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
bsp_i2c_error_t bspI2CBegin()
{
  i2cWire->begin();
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CBegin(uint8_t address)
{
  i2cWire->begin(address);
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CBeginTransmission(uint16_t address)
{
  i2cWire->beginTransmission(address);
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CBeginTransmission(uint8_t address)
{
  i2cWire->beginTransmission(address);
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CBeginTransmission(int address)
{
  i2cWire->beginTransmission(address);
  return BSP_I2C_OK;
}

uint8_t bspI2CEndTransmission(bool sendStop) { return i2cWire->endTransmission(sendStop); }

uint8_t bspI2CEndTransmission(void) { return i2cWire->endTransmission(); }

size_t bspI2CRequestFrom(uint16_t address, size_t size, bool sendStop)
{
  return i2cWire->requestFrom(address, size, sendStop);
}

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, bool sendStop)
{
  return i2cWire->requestFrom(address, size, sendStop);
}

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, uint8_t sendStop)
{
  return i2cWire->requestFrom(address, size, sendStop);
}
size_t bspI2CRequestFrom(uint8_t address, size_t len, bool stopBit)
{
  return i2cWire->requestFrom(address, len, stopBit);
}
uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size) { return i2cWire->requestFrom(address, size); }

uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size, uint8_t sendStop)
{
  return i2cWire->requestFrom(address, size, sendStop);
}
uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size) { return i2cWire->requestFrom(address, size); }

uint8_t bspI2CRequestFrom(int address, int size, int sendStop)
{
  return i2cWire->requestFrom(address, size, sendStop);
}
uint8_t bspI2CRequestFrom(int address, int size) { return i2cWire->requestFrom(address, size); }

size_t bspI2CWrite(uint8_t data) { return i2cWire->write(data); }

size_t bspI2CWrite(const uint8_t *data, size_t quantity) { return i2cWire->write(data, quantity); }

int bspI2CAvailable(void) { return i2cWire->available(); }

int bspI2CRead(void) { return i2cWire->read(); }

int bspI2CPeek(void) { return i2cWire->peek(); }

bsp_i2c_error_t bspI2CFlush(void)
{
  i2cWire->flush();
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CReadByte(uint8_t &byte, int address)
{
  int cnt = 0;
  i2cWire->requestFrom(address, 1);
  while (1 != i2cWire->available())
  {
    cnt++;
    if (cnt >= 10)
    {
      return BSP_I2C_ERR_READ;
    }
    delay(1);
  }
  byte = i2cWire->read();
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CReadBytes(uint8_t *bytes, uint32_t len, int address)
{
  int cnt = 0;
  i2cWire->requestFrom(address, len);
  while (len != i2cWire->available())
  {
    cnt++;
    if (cnt >= 10)
    {
      return BSP_I2C_ERR_READ;
    }
    delay(1);
  }
  for (int i = 0; i < len; i++)
  {
    bytes[i] = i2cWire->read();
  }
  return BSP_I2C_OK;
}

bsp_i2c_error_t bspI2CWriteByte(uint8_t byte, int address)
{
  i2cWire->beginTransmission(address);
  i2cWire->write(byte);
  return (i2cWire->endTransmission() == 0) ? BSP_I2C_OK : BSP_I2C_ERR_WRITE;
}

bsp_i2c_error_t bspI2CWriteBytes(uint8_t *bytes, uint32_t len, int address)
{
  i2cWire->beginTransmission(address);
  for (int i = 0; i < len; i++)
  {
    i2cWire->write(bytes[i]);
  }
  return (i2cWire->endTransmission() == 0) ? BSP_I2C_OK : BSP_I2C_ERR_WRITE;
}

/* Private definitions ------------------------------------------------ */
/* End of file -------------------------------------------------------- */
