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
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
// Error codes
typedef enum
{
  BSP_I2C_OK = 0,
  BSP_I2C_ERR,
  BSP_I2C_ERR_READ,
  BSP_I2C_ERR_WRITE,
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

/**
 * @brief  Reads a single byte using I2C.
 *
 * This function requests a single byte over the I2C interface
 * and stores it in the provided reference variable.
 *
 * @param[in]     None
 * @param[out]    byte  Reference to a variable where the read byte will be stored.
 * @param[inout]  None
 *
 * @attention  Ensure that the device is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the byte is not
 * immediately available.
 *
 * @return
 *  - `BSP_I2C_OK`: Success
 *  - `DHT20_ERR_I2C_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadByte(uint8_t &byte, int address);

/**
 * @brief  Reads multiple bytes using I2C.
 *
 * This function requests multiple bytes over the I2C interface
 * and stores them in the provided buffer.
 *
 * @param[in]     len  The number of bytes to read.
 * @param[out]    bytes  Pointer to a buffer where the read bytes will be stored.
 * @param[inout]  None
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *             The function includes a loop with retries and delays to handle cases where the bytes are not
 * immediately available.
 *
 * @return
 *  - `DHT20_OK`: Success
 *  - `DHT20_ERR_I2C_READ`: Error reading from the I2C bus.
 */
bsp_i2c_error_t bspI2CReadBytes(uint8_t *bytes, uint32_t len, int address);

/**
 * @brief  Writes a single byte using I2C.
 *
 * This function writes a single byte over the I2C interface.
 *
 * @param[in]     byte  The byte to write.
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *
 * @return
 *  - `DHT20_OK`: Success
 *  - `DHT20_ERR_I2C_WRITE`: Error writing to the I2C bus.
 */
bsp_i2c_error_t bspI2CWriteByte(uint8_t byte, int address);

/**
 * @brief  Writes multiple bytes using I2C.
 *
 * This function writes multiple bytes to the DHT20 sensor over the I2C interface.
 *
 * @param[in]     bytes  Pointer to a buffer containing the bytes to write.
 * @param[in]     len  The number of bytes to write.
 * @param[out]    None
 * @param[inout]  None
 *
 * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
 *
 * @return
 *  - `DHT20_OK`: Success
 *  - `DHT20_ERR_I2C_WRITE`: Error writing to the I2C bus.
 */
bsp_i2c_error_t bspI2CWriteBytes(uint8_t *bytes, uint32_t len, int address);

#endif /* BSP_I2C_H */

/* End of file -------------------------------------------------------- */
