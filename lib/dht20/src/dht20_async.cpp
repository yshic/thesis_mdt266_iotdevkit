#include "dht20_async.h"

DHT20Async::DHT20Async() : firstReading(true), lastTime(0), startMillis(0), beginState(0), resetState(0), readState(0)
{
}

// Wait for a non-blocking delay
bool DHT20Async::waitForDelay(unsigned long delayTime) { return (millis() - startMillis >= delayTime); }

// Initialize the DHT20 sensor
dht20_error_t DHT20Async::begin()
{
  switch (beginState)
  {
    case 0:
      startMillis = millis();
      NEXT_STATE(beginState, 1);
      return DHT20_OK;
    case 1:
      if (waitForDelay(100))
      { // Wait 100ms
        reset();
        RESET_STATE(beginState);
      }
      break;
  }
  return DHT20_OK;
}

// Reset the sensor
int DHT20Async::reset()
{
  static uint8_t count = 0;
  switch (resetState)
  {
    case 0:
      if (readStatus() == 0)
      {
        count++;
        if (resetRegister(0x1B))
          count++;
        if (resetRegister(0x1C))
          count++;
        if (resetRegister(0x1E))
          count++;
        NEXT_STATE(resetState, 1);
      }
      break;
    case 1:
      if (waitForDelay(10))
      { // Wait 10ms
        RESET_STATE(resetState);
        return count;
      }
      break;
  }
  return count;
}

// Reset a specific register
bool DHT20Async::resetRegister(uint8_t reg)
{
  uint8_t values[3]    = {0};
  uint8_t resetParam[] = {reg, 0x00, 0x00};

  if (i2cWriteBytes(resetParam, sizeof(resetParam)))
  {
    return false;
  }
  if (!waitForDelay(5))
    return false; // Wait 5ms

  i2cReadBytes(values, sizeof(values));
  if (!waitForDelay(10))
    return false; // Wait 10ms

  values[0] = (0xB0 | reg);
  if (i2cWriteBytes(values, sizeof(values)))
  {
    return false;
  }
  if (!waitForDelay(5))
    return false; // Wait 5ms

  return true;
}

// Read the status register
int DHT20Async::readStatus()
{
  uint8_t status = 0;
  if (i2cReadByte(status) != DHT20_OK)
  {
    DEBUG_PRINTLN("Failed to read status byte");
    return DHT20_ERR_I2C_READ;
  }
  return (status & 0x18); // Return raw status for further checks
}

// Read raw target data (temperature and humidity)
dht20_error_t DHT20Async::readTargetData(uint32_t *data)
{
  uint8_t bytes[6]       = {0};
  uint8_t configParams[] = {0xAC, 0x33, 0x00};

  switch (readState)
  {
    case 0:
      if (i2cWriteBytes(configParams, sizeof(configParams)) != DHT20_OK)
      {
        return DHT20_ERR_I2C_WRITE;
      }
      NEXT_STATE(readState, 1);
      return DHT20_OK;
    case 1:
      if (waitForDelay(80))
      { // Wait 80ms
        if (i2cReadBytes(bytes, sizeof(bytes)) != DHT20_OK)
        {
          return DHT20_ERR_I2C_READ;
        }
        if (bytes[0] & 0x80)
        {
          DEBUG_PRINTLN("Sensor is still busy");
          return DHT20_ERR_BUSY;
        }
        data[0] = ((bytes[1] << 12) | (bytes[2] << 4) | (bytes[3] >> 4));   // Humidity
        data[1] = (((bytes[3] & 0x0F) << 16) | (bytes[4] << 8) | bytes[5]); // Temperature
        RESET_STATE(readState);
      }
      break;
  }
  return DHT20_OK;
}

// Read temperature and humidity
dht20_error_t DHT20Async::readTempAndHumidity(float *data)
{
  uint32_t target_val[2] = {0};
  switch (readState)
  {
    case 0:
      if (readStatus() != 0)
      {
        return DHT20_ERR_BUSY;
      }
      if (begin() != DHT20_OK)
      {
        return DHT20_ERR_INIT;
      }
      NEXT_STATE(readState, 1);
      break;
    case 1:
      if (readTargetData(target_val) != DHT20_OK)
      {
        return DHT20_ERR_I2C_READ;
      }
      data[0] = target_val[0] * 9.5367431640625e-5;      // Convert to humidity (%RH)
      data[1] = target_val[1] * 1.9073486328125e-4 - 50; // Convert to temperature (°C)
      RESET_STATE(readState);
      break;
  }
  return DHT20_OK;
}

// Process all state machines
dht20_error_t DHT20Async::process(float *data)
{
  begin();
  reset();
  readTempAndHumidity(data); // Pass actual data buffer as needed
  return DHT20_OK;
}

/****************************************************/
/* I2C Interface */
dht20_error_t DHT20Async::i2cReadByte(uint8_t &byte)
{
  int cnt = 0;
  Wire.requestFrom(DHT20_I2C_ADDR, 1);
  while (1 != Wire.available())
  {
    cnt++;
    if (cnt >= 10)
    {
      return DHT20_ERR_I2C_READ;
    }
    delay(1);
  }
  byte = Wire.read();
  return DHT20_OK;
}

dht20_error_t DHT20Async::i2cReadBytes(uint8_t *bytes, uint32_t len)
{
  int cnt = 0;
  Wire.requestFrom(DHT20_I2C_ADDR, len);
  while (len != Wire.available())
  {
    cnt++;
    if (cnt >= 10)
    {
      return DHT20_ERR_I2C_READ;
    }
    delay(1);
  }
  for (int i = 0; i < len; i++)
  {
    bytes[i] = Wire.read();
  }
  return DHT20_OK;
}

dht20_error_t DHT20Async::i2cWriteBytes(uint8_t *bytes, uint32_t len)
{
  Wire.beginTransmission(DHT20_I2C_ADDR);
  for (int i = 0; i < len; i++)
  {
    Wire.write(bytes[i]);
  }
  return (Wire.endTransmission() == 0) ? DHT20_OK : DHT20_ERR_I2C_WRITE;
}

dht20_error_t DHT20Async::i2cWriteByte(uint8_t byte)
{
  Wire.beginTransmission(DHT20_I2C_ADDR);
  Wire.write(byte);
  return (Wire.endTransmission() == 0) ? DHT20_OK : DHT20_ERR_I2C_WRITE;
}

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */