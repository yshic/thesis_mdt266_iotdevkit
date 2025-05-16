/**
 * @file       ac_measure.cpp
 * @license    This project is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-28
 * @author     Tuan Nguyen
 *
 * @brief      Source file for ac_measure library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "ac_measure.h"
#include "bsp_i2c.h"
#include "config.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */

ac_measure_error_t AcMeasure::begin(void)
{
  if (!bspI2CExist(_addr))
  {
    return UNIT_AC_MEASURE_ERR_I2C;
  }
  return UNIT_AC_MEASURE_OK;
}
uint8_t AcMeasure::getFirmwareVersion(void)
{
  uint8_t firmwareVersion;

  bspI2CReadByte(_addr, UNIT_ACMEASURE_FIRMWARE_VERSION_REG, firmwareVersion);

  return firmwareVersion;
}

uint8_t AcMeasure::getReady(void)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_GET_READY_REG, (uint8_t *) readBuffer, 1);

  return readBuffer[0];
}

uint16_t AcMeasure::getRawVoltage(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_VOLTAGE_REG, data, 2);
  uint16_t value = data[0] | (data[1] << 8);

  return value;
}

float AcMeasure::getVoltage(void)
{
  uint16_t value = getRawVoltage();

  return (float) (value / 100.0f);
}

uint16_t AcMeasure::getRawCurrent(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_CURRENT_REG, data, 2);
  uint16_t value = data[0] | (data[1] << 8);

  return value;
}

float AcMeasure::getCurrent(void)
{
  uint16_t value = getRawCurrent();

  return (float) (value / 100.0f);
}

uint32_t AcMeasure::getRawPower(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_POWER_REG, data, 4);
  uint32_t value = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

  return value;
}

float AcMeasure::getPower(void)
{
  uint32_t value = getRawPower();

  return (float) (value / 100.0f);
}

uint32_t AcMeasure::getRawApparentPower(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_APPARENT_POWER_REG, data, 4);
  uint32_t value = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

  return value;
}

float AcMeasure::getApparentPower(void)
{
  uint32_t value = getRawApparentPower();

  return (float) (value / 100.0f);
}

uint8_t AcMeasure::getRawPowerFactor(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_POWER_FACTOR_REG, data, 1);

  return data[0];
}

float AcMeasure::getPowerFactor(void)
{
  uint8_t value = getRawPowerFactor();

  return (float) (value / 100.0f);
}

uint32_t AcMeasure::getRawKWH(void)
{
  uint8_t data[4];

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_KWH_REG, data, 4);
  uint32_t value = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);

  return value;
}

float AcMeasure::getKWH(void)
{
  uint32_t value = getRawKWH();
  return (float) (value / 100.0f);
}

ac_measure_error_t AcMeasure::getVoltageString(char *str)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_VOLTAGE_STRING_REG, (uint8_t *) readBuffer, 7);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::getCurrentString(char *str)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_CURRENT_STRING_REG, (uint8_t *) readBuffer, 7);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::getPowerString(char *str)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_POWER_STRING_REG, (uint8_t *) readBuffer, 7);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::getApparentPowerString(char *str)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_APPARENT_POWER_STRING_REG, (uint8_t *) readBuffer, 7);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::getPowerFactorString(char *str)
{
  char readBuffer[7] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_POWER_FACTOR_STRING_REG, (uint8_t *) readBuffer, 4);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::getKWHString(char *str)
{
  char readBuffer[11] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_KWH_STRING_REG, (uint8_t *) readBuffer, 11);
  memcpy(str, readBuffer, sizeof(readBuffer));

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::setKWH(uint32_t value)
{
  bspI2CWriteBytes(_addr, UNIT_ACMEASURE_KWH_REG, (uint8_t *) &value, 4);

  return UNIT_AC_MEASURE_OK;
}

uint8_t AcMeasure::getVoltageFactor(void)
{
  uint8_t data[4] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_VOLTAGE_FACTOR_REG, data, 1);

  return data[0];
}

uint8_t AcMeasure::getCurrentFactor(void)
{
  uint8_t data[4] = {0};

  bspI2CReadBytes(_addr, UNIT_ACMEASURE_CURRENT_FACTOR_REG, data, 1);

  return data[0];
}

ac_measure_error_t AcMeasure::setVoltageFactor(uint8_t value)
{
  bspI2CWriteBytes(_addr, UNIT_ACMEASURE_VOLTAGE_FACTOR_REG, (uint8_t *) &value, 1);

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::setCurrentFactor(uint8_t value)
{
  bspI2CWriteBytes(_addr, UNIT_ACMEASURE_CURRENT_FACTOR_REG, (uint8_t *) &value, 1);

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::saveVoltageCurrentFactor(void)
{
  uint8_t value = 1;

  bspI2CWriteBytes(_addr, UNIT_ACMEASURE_SAVE_FACTOR_REG, (uint8_t *) &value, 1);

  return UNIT_AC_MEASURE_OK;
}

ac_measure_error_t AcMeasure::jumpBootloader(void)
{
  uint8_t value = 1;

  bspI2CWriteBytes(_addr, UNIT_ACMEASURE_JUMP_TO_BOOTLOADER_REG, (uint8_t *) &value, 1);

  return UNIT_AC_MEASURE_OK;
}

uint8_t AcMeasure::setI2CAddress(uint8_t addr)
{
  bspI2CWriteByte(_addr, UNIT_ACMEASURE_I2C_ADDRESS_REG, addr);
  _addr = addr;
  return _addr;
}
uint8_t AcMeasure::getI2CAddress(void)
{
  uint8_t currentI2CAddress;

  bspI2CReadByte(_addr, UNIT_ACMEASURE_I2C_ADDRESS_REG, currentI2CAddress);

  return currentI2CAddress;
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */