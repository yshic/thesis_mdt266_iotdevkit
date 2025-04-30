/**
 * @file       rs485_sensors_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-12
 * @author     Tuan Nguyen
 *
 * @brief      Source file for rs485_sensors_task.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "rs485_sensors_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions-------------------------------------------- */

#ifdef ES_SOIL_RS485_MODULE
void soilRs485Task(void *pvParameters)
{
  for (;;)
  {
    esSoil.readSoilPh();
    esSoil.readSoilTempAndMoisture();
    esSoil.readSoilConductivity();
    esSoil.readSoilNPK();

    vTaskDelay(pdMS_TO_TICKS(DELAY_SOIL_RS485));
  }
}

void soilRs485Setup() { xTaskCreate(&soilRs485Task, "ES Soil Task", 4096, nullptr, 1, nullptr); }
#endif // ES_SOIL_RS485_MODULE

#ifdef RS485_MODULE
void rs485Setup() { rs485Serial1.begin(9600, SERIAL_8N1, D7, D6); }
#endif // RS485_MODULE

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */