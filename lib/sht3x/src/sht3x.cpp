/**
 * @file       sht3x.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-21
 * @author     Tuan Nguyen
 *
 * @brief      Source file for SHT3X library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "sht3x.h"
#include "bsp_i2c.h"
#include "config.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */
sht3x_error_t SHT3X::begin(uint8_t i2c_addr)
{
  _addr = i2c_addr;
  return (bspI2CExist(_addr) == true) ? SHT3X_OK : SHT3X_ERR_I2C;
}
sht3x_error_t SHT3X::update()
{
  uint8_t data[6];

  bspI2CWriteByte(_addr, 0x2C, 0x06); // 0x2C : Clock stretching ON
                                      // 0x06 : Repeatability HIGH
  DELAY(200);

  // Read 6 bytes of data
  // cTemp msb, cTemp lsb, cTemp crc, humidity msb, humidity lsb, humidity crc
  bspI2CReadBytes(_addr, data, sizeof(data));

  DELAY(50);

  // Convert the data
  sensorValues[SHT3X_TEMPERATURE_INDEX] = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
  sensorValues[SHT3X_HUMIDITY_INDEX]    = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);

  return SHT3X_OK;
}

float SHT3X::getHumidity() { return sensorValues[SHT3X_HUMIDITY_INDEX]; }

float SHT3X::getTemperatureCelcius() { return sensorValues[SHT3X_TEMPERATURE_INDEX]; }

float SHT3X::getTemperatureFahrenheit() { return ((sensorValues[SHT3X_TEMPERATURE_INDEX] * 1.8) + 32); }

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */