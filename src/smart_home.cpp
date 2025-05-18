/**
 * @file       smart_home.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-28
 * @author     Tuan Nguyen
 *
 * @brief      Source file for smart_home.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "smart_home.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */
void SmartHome::serialSetup(bool uart, bool rs485, bool i2c)
{
  Serial.begin(9600);

  if (uart)
  {
    uartSetup();
  }

  if (rs485)
  {
    rs485Setup();
  }

  if (i2c)
  {
    Wire.begin(SDA, SCL, 400000U);
  }
}

void SmartHome::deviceEnable(const bool status[11])
{
  for (uint8_t i = 0; i < 11; i++)
  {
    deviceStatus[i] = status[i];
  }
}

void SmartHome::connectivityEnable(const bool status[2])
{
  for (uint8_t i = 0; i < 11; i++)
  {
    connectivity[i] = status[i];
  }
}

void SmartHome::deviceSetup()
{
  for (uint8_t i = 0; i < 11; i++)
  {
    if (deviceStatus[i])
    {
      deviceInitialize(i);
    }
  }
}

void SmartHome::connectivitySetup()
{
  for (uint8_t i = 0; i < 2; i++)
  {
    if (connectivity[i])
    {
      connectivityInitialize(i);
    }
  }
}

void SmartHome::iotServerConnect() { iotServerSetup(); }

void SmartHome::deviceInitialize(uint8_t index)
{
  switch (index)
  {
    case DEVICE_INDEX_ENV_IV:
      unitENVIVSetup();
      break;

    case DEVICE_INDEX_LIGHT_SENSOR:
      lightSensorSetup();
      break;

    case DEVICE_INDEX_PIR_SENSOR:
      pirSensorSetup();
      break;

    case DEVICE_INDEX_ES_SOIL:
      soilRs485Setup();
      break;

    case DEVICE_INDEX_LCD:
      lcdSetup();
      break;

    case DEVICE_INDEX_4_RELAY:
      unit4RelaySetup();
      break;

    case DEVICE_INDEX_BUTTON:
      buttonSetup();
      break;

    case DEVICE_INDEX_SERVO:
      doorSetup();
      break;

    case DEVICE_INDEX_MINI_FAN:
      break;

    case DEVICE_INDEX_AC_MEASURE:
      // acMeasureSetup();
      break;

    case DEVICE_INDEX_HUSKYLENS:
      huskylensSetup();
      break;

    default:
      break;
  }
}

void SmartHome::connectivityInitialize(uint8_t index)
{
  switch (index)
  {
    case CONNECTIVITY_WIFI_INDEX:
      wifiSetup();
      break;

    case CONNECTIVITY_BLE_INDEX:
      bleSetup();
      break;

    default:
      break;
  }
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */