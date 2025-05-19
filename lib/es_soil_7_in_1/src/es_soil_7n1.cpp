/**
 * @file       es_soil_7n1
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-13
 * @author     Tuan Nguyen
 *
 * @brief      Source file for es_soil_7n1 library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "es_soil_7n1.h"
#include "bsp_rs485.h"
#include "config.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions-------------------------------------------- */

es_soil_7n1_error_t EsSoil7n1::readSoilPh()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B}; // Reading pH
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilPh: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract pH
  uint16_t rawPh  = (response[3] << 8) | response[4];
  sensorValues[0] = rawPh * 0.01;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilMoisture()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0F}; // Reading Moisture
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilMoisture: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract moisture
  uint16_t rawMoisture = (response[3] << 8) | response[4];
  sensorValues[1]      = rawMoisture * 0.1;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilTemperature()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF}; // Reading Temperature
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilTemperature: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract temperature
  uint16_t rawTemperature = (response[3] << 8) | response[4];
  sensorValues[2]         = rawTemperature * 0.1;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilConductivity()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xCE}; // Reading Conductivity
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilConductivity: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract conductivity
  uint16_t rawConductivity = (response[3] << 8) | response[4];
  sensorValues[3]          = rawConductivity;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilNitrogen()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xE4, 0x0C}; // Reading Nitrogen
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilNitrogen: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract nitrogen
  uint16_t rawNitrogen = (response[3] << 8) | response[4];
  sensorValues[4]      = rawNitrogen;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilPhosphorus()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xB5, 0xCC}; // Reading Phosphorus
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilPhosphorus: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract phosphorus
  uint16_t rawPhosphorus = (response[3] << 8) | response[4];
  sensorValues[5]        = rawPhosphorus;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilPotassium()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xC0}; // Reading Potassium
  byte response[7];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilPotassium: ");
  for (int i = 0; i < 7; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x02)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract potassium
  uint16_t rawPotassium = (response[3] << 8) | response[4];
  sensorValues[6]       = rawPotassium;

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilTempAndMoisture()
{
  byte command[8] = {
  ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x12, 0x00, 0x02, 0x64, 0x0E}; // Reading Temperature and Moisture
  byte response[9];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilTempAndMoisture: ");
  for (int i = 0; i < 9; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  // Verify the response format
  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x04)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract moisture and temperature (high byte first)
  uint16_t soilHumidityRaw    = (response[3] << 8) | response[4]; // e.g., 01F4H = 500
  uint16_t soilTemperatureRaw = (response[5] << 8) | response[6]; // e.g., 00C8H = 200

  // Convert to physical units
  sensorValues[1] = soilHumidityRaw * 0.1;    // Unit: 0.1% RH → 50.0% RH
  sensorValues[2] = soilTemperatureRaw * 0.1; // Unit: 0.1°C → 20.0°C

  return ES_SOIL_7N1_OK;
}

es_soil_7n1_error_t EsSoil7n1::readSoilNPK()
{
  byte command[8] = {ES_SOIL_SLAVE_ID, 0x03, 0x00, 0x1E, 0x00, 0x03, 0x65, 0xCD}; // Reading N-P-K
  byte response[11];

  rs485Serial1.sendModbusCommand(command, sizeof(command), response, sizeof(response));

#ifdef DEBUG_PRINT_ES_SOIL_RAW_RESPONSE
  Serial.print("Raw response SoilNPK: ");
  for (int i = 0; i < 11; i++)
  {
    Serial.print(response[i], HEX);
    Serial.print(" ");
  }
#endif // DEBUG_PRINT_ES_SOIL_RAW_RESPONSE

  // Verify the response format
  if (response[0] != ES_SOIL_SLAVE_ID || response[1] != 0x03 || response[2] != 0x06)
  {
#ifdef DEBUG_PRINT
    Serial.println("Error: Invalid response format");
#endif // DEBUG_PRINT

    return ES_SOIL_7N1_ERR;
  }

  // Extract nitrogen, phosphorus and potassium (high byte first)
  uint16_t soilNitrogenRaw   = (response[3] << 8) | response[4]; // e.g., 0020H = 32
  uint16_t soilPhosphorusRaw = (response[5] << 8) | response[6]; // e.g., 0025H = 37
  uint16_t soilPotassiumRaw  = (response[7] << 8) | response[8]; // e.g., 0030H = 48

  // Convert to physical units
  sensorValues[4] = soilNitrogenRaw;   // Unit: 32 mg/kg
  sensorValues[5] = soilPhosphorusRaw; // Unit: 37 mg/kg
  sensorValues[6] = soilPotassiumRaw;  // Unit: 48 mg/kg

  return ES_SOIL_7N1_OK;
}

float EsSoil7n1::getSoilPh() { return sensorValues[0]; }

float EsSoil7n1::getSoilMoisture() { return sensorValues[1]; }

float EsSoil7n1::getSoilTemperature() { return sensorValues[2]; }

float EsSoil7n1::getSoilConductivity() { return sensorValues[3]; }

float EsSoil7n1::getSoilNitrogen() { return sensorValues[4]; }

float EsSoil7n1::getSoilPhosphorus() { return sensorValues[5]; }

float EsSoil7n1::getSoilPotassium() { return sensorValues[6]; }

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */