/**
 * @file       sensors_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-12
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Sensors Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "sensors_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
float temperature;
float humidity;

/* Task definitions ------------------------------------------- */
void dht20Task(void *pvParameters)
{
  for (;;)
  {
    dht20.readTempAndHumidity();
    vTaskDelay(DELAY_DHT20 / portTICK_PERIOD_MS);
  }
}
void lightSensorTask(void *pvParameters)
{
  for (;;)
  {
    lightSensor.read();
    vTaskDelay(DELAY_LIGHT_SENSOR / portTICK_PERIOD_MS);
  }
}
void ultrasonicTask(void *pvParameters)
{
  for (;;)
  {
    ultrasonic.read('c');
    ultrasonic.read('m');
    ultrasonic.read('i');
    vTaskDelay(DELAY_ULTRASONIC / portTICK_PERIOD_MS);
  }
}
void pirSensorTask(void *pvParameters)
{
  for (;;)
  {
    pirSensor.read();
    vTaskDelay(DELAY_PIRSENSOR / portTICK_PERIOD_MS);
  }
}

void soilMoistureTask(void *pvParameters)
{
  for (;;)
  {
    soilMoisture.read();
    vTaskDelay(DELAY_MOISTURE / portTICK_PERIOD_MS);
  }
}

void dht20Setup()
{
  dht20.begin();
  xTaskCreate(dht20Task, "DHT 20 Task", 1024, NULL, 1, NULL);
}

void lightSensorSetup() { xTaskCreate(lightSensorTask, "Light Sensor Task", 1024, NULL, 1, NULL); }

void ultrasonicSetup() { xTaskCreate(ultrasonicTask, "Ultrasonic Sensor Task", 1024, NULL, 2, NULL); }

void pirSensorSetup() { xTaskCreate(pirSensorTask, "PIR Sensor Task", 1024, NULL, 2, NULL); }

void soilMoistureSetup() { xTaskCreate(soilMoistureTask, "Soil Moisture Task", 1024, NULL, 1, NULL); }

/* End of file -------------------------------------------------------- */
