/**
 * @file       ac_measure.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-28
 * @author     Tuan Nguyen
 *
 * @brief      Header file for AC_MEASURE library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef AC_MEASURE_H
  #define AC_MEASURE_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define AC_MEASURE_LIB_VERSION                   (F("0.1.0"))

  #define UNIT_ACMEASURE_DEFAULT_ADDR              0x42
  #define UNIT_ACMEASURE_VOLTAGE_STRING_REG        0x00
  #define UNIT_ACMEASURE_CURRENT_STRING_REG        0x10
  #define UNIT_ACMEASURE_POWER_STRING_REG          0x20
  #define UNIT_ACMEASURE_APPARENT_POWER_STRING_REG 0x30
  #define UNIT_ACMEASURE_POWER_FACTOR_STRING_REG   0x40
  #define UNIT_ACMEASURE_KWH_STRING_REG            0x50
  #define UNIT_ACMEASURE_VOLTAGE_REG               0x60
  #define UNIT_ACMEASURE_CURRENT_REG               0x70
  #define UNIT_ACMEASURE_POWER_REG                 0x80
  #define UNIT_ACMEASURE_APPARENT_POWER_REG        0x90
  #define UNIT_ACMEASURE_POWER_FACTOR_REG          0xA0
  #define UNIT_ACMEASURE_KWH_REG                   0xB0
  #define UNIT_ACMEASURE_VOLTAGE_FACTOR_REG        0xC0
  #define UNIT_ACMEASURE_CURRENT_FACTOR_REG        0xD0
  #define UNIT_ACMEASURE_SAVE_FACTOR_REG           0xE0
  #define UNIT_ACMEASURE_GET_READY_REG             0xFC
  #define UNIT_ACMEASURE_JUMP_TO_BOOTLOADER_REG    0xFD
  #define UNIT_ACMEASURE_FIRMWARE_VERSION_REG      0xFE
  #define UNIT_ACMEASURE_I2C_ADDRESS_REG           0xFF

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  UNIT_AC_MEASURE_OK = 0,   /* No error */
  UNIT_AC_MEASURE_ERR,      /* Generic error */
  UNIT_AC_MEASURE_ERR_INIT, /* Initialization error */
  UNIT_AC_MEASURE_TIMEOUT,  /* Timeout error*/
  UNIT_AC_MEASURE_ERR_I2C,  /* I2C error */
} ac_measure_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class AcMeasure
{
private:
  uint8_t _addr = UNIT_ACMEASURE_DEFAULT_ADDR;

public:
  ac_measure_error_t begin(void);
  uint8_t            getFirmwareVersion(void);
  uint8_t            getReady(void);

  uint16_t getRawVoltage(void);
  float getVoltage(void);
  uint16_t getRawCurrent(void);
  float getCurrent(void);
  uint32_t getRawPower(void);
  float getPower(void);
  uint32_t getRawApparentPower(void);
  float getApparentPower(void);
  uint8_t  getRawPowerFactor(void);
  float  getPowerFactor(void);
  uint32_t getRawKWH(void);
  float getKWH(void);

  ac_measure_error_t getVoltageString(char *str);
  ac_measure_error_t getCurrentString(char *str);
  ac_measure_error_t getPowerString(char *str);
  ac_measure_error_t getApparentPowerString(char *str);
  ac_measure_error_t getPowerFactorString(char *str);
  ac_measure_error_t getKWHString(char *str);
  ac_measure_error_t setKWH(uint32_t value);

  uint8_t            getVoltageFactor(void);
  uint8_t            getCurrentFactor(void);
  ac_measure_error_t setVoltageFactor(uint8_t value);
  ac_measure_error_t setCurrentFactor(uint8_t value);
  ac_measure_error_t saveVoltageCurrentFactor(void);

  ac_measure_error_t jumpBootloader(void);
  uint8_t            setI2CAddress(uint8_t addr);
  uint8_t            getI2CAddress(void);
};

#endif // AC_MEASURE_H

/* End of file -------------------------------------------------------- */