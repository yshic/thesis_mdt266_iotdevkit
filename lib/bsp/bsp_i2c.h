/**
 * @file       bsp_i2c.h
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for bsp_i2c
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_I2C_H
  #define BSP_I2C_H

  /* Includes --------------------------------------------------------- */
  #include <Arduino.h>

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
// Error codes
typedef enum
{
  BSP_I2C_OK = 0,
  BSP_I2C_ERR,
  BSP_I2C_TIMEOUT
} bsp_i2c_error_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
bsp_i2c_error_t bspI2CBegin();
bsp_i2c_error_t bspI2CBegin(uint8_t address);

// beginTransmission
bsp_i2c_error_t bspI2CBeginTransmission(uint16_t address);

bsp_i2c_error_t bspI2CBeginTransmission(uint8_t address);

bsp_i2c_error_t bspI2CBeginTransmission(int address);

// endTransmission
uint8_t bspI2CEndTransmission(bool sendStop);

uint8_t bspI2CEndTransmission(void);

// requestFrom
size_t bspI2CRequestFrom(uint16_t address, size_t size, bool sendStop);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, bool sendStop);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size, uint8_t sendStop);

size_t bspI2CRequestFrom(uint8_t address, size_t len, bool stopBit);

uint8_t bspI2CRequestFrom(uint16_t address, uint8_t size);

uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size, uint8_t sendStop);

uint8_t bspI2CRequestFrom(uint8_t address, uint8_t size);

uint8_t bspI2CRequestFrom(int address, int size, int sendStop);

uint8_t bspI2CRequestFrom(int address, int size);

// write
size_t bspI2CWrite(uint8_t);

size_t bspI2CWrite(const uint8_t *, size_t);

int bspI2CAvailable(void);

int bspI2CRead(void);

int bspI2CPeek(void);

bsp_i2c_error_t bspI2CFlush(void);
#endif /* BSP_I2C_H */

/* End of file -------------------------------------------------------- */
