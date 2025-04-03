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

/* Task definitions ------------------------------------------- */
#ifdef DHT20_MODULE
void dht20Task(void *pvParameters)
{
  for (;;)
  {
    dht20.readTempAndHumidity();
    vTaskDelay(DELAY_DHT20 / portTICK_PERIOD_MS);
  }
}

void dht20Setup()
{
  dht20.begin();
  xTaskCreate(dht20Task, "DHT 20 Task", 8192, NULL, 1, NULL);
}
#endif // DHT20_MODULE

#ifdef SHT4X_MODULE
void sht40Task(void *pvParameters)
{
  for (;;)
  {
    sht40.update();
    vTaskDelay(DELAY_SHT4X / portTICK_PERIOD_MS);
  }
}
void sht40Setup()
{
  sht40.begin();
  sht40.setHeater(SHT4X_NO_HEATER);
  sht40.setPrecision(SHT4X_HIGH_PRECISION);
  xTaskCreate(sht40Task, "SHT40 Task", 8192, NULL, 1, NULL);
}
#endif // SHT4X_MODULE

#ifdef BMP280_MODULE
void bmp280Task(void *pvParameters)
{
  for (;;)
  {
    bmp280.update();
    vTaskDelay(DELAY_BMP280 / portTICK_PERIOD_MS);
  }
}

void bmp280Setup()
{
  bmp280.begin();
  bmp280.setSampling(MODE_NORMAL,     /* Operating Mode. */
                     SAMPLING_X2,     /* Temp. oversampling */
                     SAMPLING_X16,    /* Pressure oversampling */
                     FILTER_X16,      /* Filtering. */
                     STANDBY_MS_500); /* Standby time. */
  xTaskCreate(bmp280Task, "BMP280 Task", 8192, NULL, 1, NULL);
}
#endif // BMP280_MODULE

#if defined(SHT4X_MODULE) && defined(BMP280_MODULE)
void unitENVIVSetup()
{
  sht40Setup();
  bmp280Setup();
}
#endif // defined(SHT4X_MODULE) && defined(BMP280_MODULE)

#ifdef AC_MEASURE_MODULE
void acMeasureSetup() { acMeasure.begin(); }
#endif // AC_MEASURE_MODULE

#ifdef LIGHT_SENSOR_MODULE
void lightSensorTask(void *pvParameters)
{
  for (;;)
  {
    lightSensor.read();
    vTaskDelay(DELAY_LIGHT_SENSOR / portTICK_PERIOD_MS);
  }
}

void lightSensorSetup() { xTaskCreate(lightSensorTask, "Light Sensor Task", 4096, NULL, 2, NULL); }
#endif // LIGHT_SENSOR_MODULE

#ifdef ULTRASONIC_MODULE
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
void ultrasonicSetup() { xTaskCreate(ultrasonicTask, "Ultrasonic Sensor Task", 4096, NULL, 2, NULL); }
#endif // ULTRASONIC_MODULE

#ifdef PIR_MODULE
void pirSensorTask(void *pvParameters)
{
  for (;;)
  {
    pirSensor.read();
    vTaskDelay(DELAY_PIRSENSOR / portTICK_PERIOD_MS);
  }
}
void pirSensorSetup() { xTaskCreate(pirSensorTask, "PIR Sensor Task", 4096, NULL, 2, NULL); }
#endif // PIR_MODULE

#ifdef SOIL_MOISTURE_MODULE
void soilMoistureTask(void *pvParameters)
{
  for (;;)
  {
    soilMoisture.read();
    vTaskDelay(DELAY_MOISTURE / portTICK_PERIOD_MS);
  }
}
void soilMoistureSetup() { xTaskCreate(soilMoistureTask, "Soil Moisture Task", 4096, NULL, 1, NULL); }
#endif // SOIL_MOISTURE_MODULE

/* End of file -------------------------------------------------------- */
