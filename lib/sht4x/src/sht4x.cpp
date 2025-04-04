/**
 * @file       sht4x.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-21
 * @author     Tuan Nguyen
 *
 * @brief      Source file for SHT4x library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "sht4x.h"
#include "bsp_i2c.h"
#include "config.h"
#include "utility.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions ------------------------------------------- */

sht4x_error_t SHT4X::begin() { return (bspI2CExist(SHT40_I2C_ADDR_44) == true) ? SHT4X_OK : SHT4X_ERR_I2C; }

sht4x_error_t SHT4X::update()
{
  uint8_t  readBuffer[6];
  uint8_t  cmd      = SHT4x_CMD_NOHEAT_HIGHPRECISION;
  uint16_t duration = 10;
  switch (_heater)
  {
    case SHT4X_NO_HEATER:
      switch (_precision)
      {
        case SHT4X_HIGH_PRECISION:
          cmd      = SHT4x_CMD_NOHEAT_HIGHPRECISION;
          duration = 10;
          break;
        case SHT4X_MED_PRECISION:
          cmd      = SHT4x_CMD_NOHEAT_MEDPRECISION;
          duration = 5;
          break;
        case SHT4X_LOW_PRECISION:
          cmd      = SHT4x_CMD_NOHEAT_LOWPRECISION;
          duration = 2;
          break;
      }
      break;
    case SHT4X_HIGH_HEATER_1S:
      cmd      = SHT4x_CMD_HIGHHEAT_1S;
      duration = 1100;
      break;
    case SHT4X_HIGH_HEATER_100MS:
      cmd      = SHT4x_CMD_HIGHHEAT_100MS;
      duration = 110;
      break;
    case SHT4X_MED_HEATER_1S:
      cmd      = SHT4x_CMD_MEDHEAT_1S;
      duration = 1100;
      break;
    case SHT4X_MED_HEATER_100MS:
      cmd      = SHT4x_CMD_MEDHEAT_100MS;
      duration = 110;
      break;
    case SHT4X_LOW_HEATER_1S:
      cmd      = SHT4x_CMD_LOWHEAT_1S;
      duration = 1100;
      break;
    case SHT4X_LOW_HEATER_100MS:
      cmd      = SHT4x_CMD_LOWHEAT_100MS;
      duration = 110;
      break;
  }
  bspI2CWriteByte(SHT40_I2C_ADDR_44, cmd, 1);

  DELAY(duration);

  bspI2CReadBytes(SHT40_I2C_ADDR_44, readBuffer, sizeof(readBuffer));

  if (readBuffer[2] != crc8(readBuffer, 2) || readBuffer[5] != crc8(readBuffer + 3, 2))
  {
    return SHT4X_ERR_CHECKSUM;
  }
  float t_ticks  = (uint16_t) readBuffer[0] * 256 + (uint16_t) readBuffer[1];
  float rh_ticks = (uint16_t) readBuffer[3] * 256 + (uint16_t) readBuffer[4];

  sensorValue[SHT4X_HUMIDITY_INDEX] = -6 + 125 * rh_ticks / 65535;

  sensorValue[SHT4X_HUMIDITY_INDEX] = min(max(sensorValue[SHT4X_HUMIDITY_INDEX], (float) 0.0), (float) 100.0);

  sensorValue[SHT4X_TEMPERATURE_INDEX] = -45 + 175 * t_ticks / 65535;

  return SHT4X_OK;
}

void SHT4X::setPrecision(sht4x_precision_t prec) { _precision = prec; }

sht4x_precision_t SHT4X::getPrecision() { return _precision; }

void SHT4X::setHeater(sht4x_heater_t heat) { _heater = heat; }

sht4x_heater_t SHT4X::getHeater() { return _heater; }

float SHT4X::getTemperature() { return sensorValue[SHT4X_TEMPERATURE_INDEX]; }

float SHT4X::getHumidity() { return sensorValue[SHT4X_HUMIDITY_INDEX]; }
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
