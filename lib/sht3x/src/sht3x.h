/**
 * @file       sht3x.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-21
 * @author     Tuan Nguyen
 *
 * @brief      Header file for SHT3X library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef SHT3X_H
  #define SHT3X_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define SHT3X_LIB_VERSION       (F("0.1.0"))

  #define SHT3X_I2C_ADDR_DEFAULT  0x44
  #define SHT3X_HUMIDITY_INDEX    0
  #define SHT3X_TEMPERATURE_INDEX 1

/* Public enumerate/structure ----------------------------------------- */

typedef enum
{
  SHT3X_OK = 0,      /* No error */
  SHT3X_ERR,         /* Generic error */
  SHT3X_ERR_INIT,    /* Initialization error */
  SHT3X_ERR_RESET,   /* Reset error */
  SHT3X_TIMEOUT,     /* Timeout error*/
  SHT3X_ERR_I2C,     /* I2C error */
  SHT3X_ERR_CHECKSUM /* Checksum error */
} sht3x_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class SHT3X
{
public:
  sht3x_error_t begin(uint8_t i2c_addr = SHT3X_I2C_ADDR_DEFAULT);
  sht3x_error_t update(void);

  float getHumidity();
  float getTemperatureCelcius();
  float getTemperatureFahrenheit();

private:
  float sensorValues[2] = {0}; // index 0 : Humidity
                               // index 1 : Temperature
  uint8_t _addr;
};

#endif // SHT3X_H

/* End of file -------------------------------------------------------- */