/**
 * @file       dht20.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for DHT20 library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "dht20.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions ------------------------------------------- */

// Constructor
DHT20::DHT20() {}

dht20_error_t DHT20::begin()
{
  delay(100); // Wait 100ms after power-on
  reset();
  return DHT20_OK;
}

/**** The logic for reseting the sensor is taken from another source with some modifications ****/
// Author: Rob Tillaart
// Date: 2024
// URL: https://github.com/RobTillaart/DHT20

int DHT20::reset(void)
{
  uint8_t count = 255;

  if (readStatus() == 0)
  {
    count++;
    if (_resetRegister(0x1B))
      count++;
    if (_resetRegister(0x1C))
      count++;
    if (_resetRegister(0x1E))
      count++;
    delay(10);
  }
  return count;
}

bool DHT20::_resetRegister(uint8_t reg)
{
  uint8_t values[3]    = {0};
  uint8_t resetParam[] = {reg, 0x00, 0x00};
  if (i2cWriteBytes(resetParam, sizeof(resetParam)))
  {
    return false;
  }
  delay(5);

  i2cReadBytes(values, sizeof(values));
  delay(10);

  values[0] = (0xB0 | reg);
  if (i2cWriteBytes(values, sizeof(values)))
  {
    return false;
  }
  delay(5);

  return true;
}
/**** END ****/

int DHT20::readStatus(void)
{
  int     returnVal = 0;
  uint8_t status    = 0;

  returnVal = i2cReadByte(status);
  if (returnVal)
  {
    Serial.println("Failed to read byte\n");
  }
  if ((status & 0x18) == 0x18)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

dht20_error_t DHT20::readTargetData(uint32_t *data)
{
  uint8_t bytes[6]       = {0};                // Buffer to hold the 6 bytes
  uint8_t configParams[] = {0xAC, 0x33, 0x00}; // Command to start measurement

  // Send the measurement command
  if (i2cWriteBytes(configParams, sizeof(configParams)) != DHT20_OK)
  {
    return DHT20_ERR_I2C_WRITE; // Write failed
  }

  delay(80); // Wait for measurement to complete

  // Read 6 bytes from the sensor
  if (i2cReadBytes(bytes, sizeof(bytes)) != DHT20_OK)
  {
    return DHT20_ERR_I2C_READ; // Read failed
  }

  // Check the busy flag in the state byte (bytes[0])
  if (bytes[0] & 0x80)
  {
    Serial.println("Sensor is still busy");
    return DHT20_ERR; // Sensor not ready
  }

  // Process humidity data (20 bits: bytes[1], bytes[2], bytes[3])
  data[HUMIDITY_INDEX] = ((bytes[1] << 12) | (bytes[2] << 4) | (bytes[3] >> 4));

  // Process temperature data (20 bits: bytes[3], bytes[4], bytes[5])
  data[TEMPERATURE_INDEX] = (((bytes[3] & 0x0F) << 16) | (bytes[4] << 8) | bytes[5]);

  return DHT20_OK; // Success
}

dht20_error_t DHT20::readTempAndHumidity()
{
  uint32_t target_val[2] = {0};
  uint32_t cnt           = 0;
  while (readStatus() == 0)
  {
    begin();
    delay(30);
    cnt++;
    if (cnt > 3)
    {
      return DHT20_TIMEOUT;
    }
  }
  // wait for data ready。
  while (readTargetData(target_val))
  {
    cnt++;
    delay(50);
    if (cnt > 5)
    {
      return DHT20_TIMEOUT;
    }
  }
  sensorValue[HUMIDITY_INDEX] =
  target_val[0] * 9.5367431640625e-5; // equivalent to target_val[0] * 100.0 / 1024 / 1024 but more
                                      // optimize by not using division
  sensorValue[TEMPERATURE_INDEX] =
  target_val[1] * 1.9073486328125e-4 - 50; // target_val[1] * 200.0 / 1024 / 1024 - 50
  return DHT20_OK;
}

float DHT20::getHumidity() { return sensorValue[HUMIDITY_INDEX]; }

float DHT20::getTemperature() { return sensorValue[TEMPERATURE_INDEX]; }

/****************************************************/
/* I2C Interface */
dht20_error_t DHT20::i2cReadByte(uint8_t &byte)
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

dht20_error_t DHT20::i2cReadBytes(uint8_t *bytes, uint32_t len)
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

dht20_error_t DHT20::i2cWriteBytes(uint8_t *bytes, uint32_t len)
{
  Wire.beginTransmission(DHT20_I2C_ADDR);
  for (int i = 0; i < len; i++)
  {
    Wire.write(bytes[i]);
  }
  return (Wire.endTransmission() == 0) ? DHT20_OK : DHT20_ERR_I2C_WRITE;
}

dht20_error_t DHT20::i2cWriteByte(uint8_t byte)
{
  Wire.beginTransmission(DHT20_I2C_ADDR);
  Wire.write(byte);
  return (Wire.endTransmission() == 0) ? DHT20_OK : DHT20_ERR_I2C_WRITE;
}

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
