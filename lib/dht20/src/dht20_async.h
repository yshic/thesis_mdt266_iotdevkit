#ifndef DHT20_ASYNC_H
#define DHT20_ASYNC_H

/* Includes ----------------------------------------------------------- */
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Wire.h"

/* Public defines ----------------------------------------------------- */
#define DHT20_LIB_VERSION (F("0.1.0"))

#define DHT20_I2C_ADDR    0x38
#define HUMIDITY_INDEX    0
#define TEMPERATURE_INDEX 1

/* Public enumerate/structure ----------------------------------------- */
enum dht20_error_t
{
  DHT20_OK = 0,
  DHT20_ERR_I2C_WRITE,
  DHT20_ERR_I2C_READ,
  DHT20_ERR_BUSY,
  DHT20_ERR_INIT,
  DHT20_ERR_DELAY
};

/* Public macros ------------------------------------------------------ */

// Macros for state management
#define NEXT_STATE(state, next) \
  {                             \
    state       = next;         \
    startMillis = millis();     \
  }
#define RESET_STATE(state) \
  {                        \
    state = 0;             \
  }

// Debug macros
#define DEBUG_PRINT(x)   Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

class DHT20Async
{
public:
  DHT20Async();

  dht20_error_t begin();                          // Initialize the sensor
  int           reset();                          // Reset the sensor
  int           readStatus();                     // Read the status register
  dht20_error_t readTargetData(uint32_t *data);   // Read raw target data (temp & humidity)
  dht20_error_t readTempAndHumidity(float *data); // Read temperature and humidity
  dht20_error_t process(float *data);             // Main loop for state machine handling

private:
  // State variables
  bool          firstReading;
  unsigned long lastTime;
  unsigned long startMillis;
  int           beginState;
  int           resetState;
  int           readState;

  /**
   * @brief  Reads a single byte from the DHT20 sensor using I2C.
   *
   * This function requests a single byte from the DHT20 sensor over the I2C interface
   * and stores it in the provided reference variable.
   *
   * @param[in]     None
   * @param[out]    byte  Reference to a variable where the read byte will be stored.
   * @param[inout]  None
   *
   * @attention  Ensure that the sensor is correctly connected and that I2C communication is functioning.
   *             The function includes a loop with retries and delays to handle cases where the byte is not
   * immediately available.
   *
   * @return
   *  - `DHT20_OK`: Success
   *  - `DHT20_ERR_I2C_READ`: Error reading from the I2C bus.
   */
  dht20_error_t i2cReadByte(uint8_t &byte);

  /**
   * @brief  Reads multiple bytes from the DHT20 sensor using I2C.
   *
   * This function requests multiple bytes from the DHT20 sensor over the I2C interface
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
  dht20_error_t i2cReadBytes(uint8_t *bytes, uint32_t len);

  /**
   * @brief  Writes multiple bytes to the DHT20 sensor using I2C.
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
  dht20_error_t i2cWriteBytes(uint8_t *bytes, uint32_t len);

  /**
   * @brief  Writes a single byte to the DHT20 sensor using I2C.
   *
   * This function writes a single byte to the DHT20 sensor over the I2C interface.
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
  dht20_error_t i2cWriteByte(uint8_t byte);

  // Internal helpers
  bool waitForDelay(unsigned long delayTime);
  bool resetRegister(uint8_t reg);
};

#endif // DHT20_ASYNC_H
