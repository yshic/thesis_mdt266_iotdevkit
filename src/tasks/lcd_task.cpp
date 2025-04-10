/**
 * @file       lcd_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for LCD Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "lcd_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions ------------------------------------------- */
#ifdef LCD_MODULE
void lcdTask(void *pvParameters)
{
  for (;;)
  {
    if (wifiConnected)
    {
      switch (lcd.getScreenState())
      {
  #ifdef DHT20_MODULE
        case LCD_SCREEN_DHT20:
          lcd.clear();
          lcd.print("Hum: ");
          lcd.print(dht20.getHumidity());
          lcd.print(" %");
          lcd.setCursor(0, 1);
          lcd.print("Temp: ");
          lcd.print(dht20.getTemperature());
          lcd.print(" *C");
          lcd.setScreenState(LCD_SCREEN_LIGHT);
          break;
  #endif

  #ifdef SHT4X_MODULE
        case LCD_SCREEN_SHT4X:
          lcd.clear();
          lcd.print("Hum: ");
          lcd.print(sht40.getHumidity());
          lcd.print(" %");
          lcd.setCursor(0, 1);
          lcd.print("Temp: ");
          lcd.print(sht40.getTemperature());
          lcd.print(" *C");
          lcd.setScreenState(LCD_SCREEN_LIGHT);
          //  NOTE: RTOS BREAK instr ERROR when use updateScreenState();
          break;
  #endif

  #ifdef LIGHT_SENSOR_MODULE
        case LCD_SCREEN_LIGHT:
          lcd.clear();
          lcd.print("Light level: ");
          lcd.print(lightSensor.getLightValuePercentage());
          lcd.progressBar(1, lightSensor.getLightValuePercentage());
          lcd.setScreenState(LCD_SCREEN_ULTRASONIC);
          break;
  #endif

  #ifdef ULTRASONIC_MODULE
        case LCD_SCREEN_ULTRASONIC:
          lcd.clear();
          lcd.print("Distance: ");
          lcd.print(ultrasonic.getDistance('c'));
          lcd.print(" cm");
          lcd.setScreenState(LCD_SCREEN_DHT20);
          break;
  #endif

  #ifdef SOIL_MOISTURE_MODULE
        case LCD_SCREEN_MOISTURE:
          lcd.clear();
          lcd.print("Moisture: ");
          lcd.print(soilMoisture.getMoisture());
          lcd.setCursor(0, 1);
          lcd.print(soilMoisture.getMoisturePercentage());
          lcd.print(" %");
          break;
  #endif

  #ifdef PIR_MODULE
        case LCD_SCREEN_PIR:
          lcd.clear();
          lcd.print(pirSensor.getStatus() ? "Motion Detected!" : "No Motion!");
          break;
  #endif

  #ifdef MINI_FAN_MODULE
        case LCD_SCREEN_MINIFAN:
          lcd.clear();
          lcd.print("Fan Speed: ");
          lcd.print(miniFan.getFanSpeed());
          lcd.setCursor(0, 1);
          lcd.print(miniFan.getFanSpeedPercentage());
          break;
  #endif

        default:
          lcd.clear();
          break;
      }
    }
    vTaskDelay(DELAY_LCD / portTICK_PERIOD_MS);
  }
}

void lcdSetup()
{
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.clear();
  xTaskCreate(lcdTask, "LCD Task", 8192, NULL, 2, NULL);
}
#endif // LCD_MODULE

/* End of file -------------------------------------------------------- */