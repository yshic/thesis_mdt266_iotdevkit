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
HUSKYLENSResult _result;
bool            doorState = false;
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
          break;
  #endif

  #ifdef SERVO_MODULE
        case LCD_SCREEN_DOOR:
          doorState = doorServo.getDoorStatus();
          lcd.clear();
          lcd.print("Door Status: ");
          lcd.setCursor(0, 1);
          if (doorState)
          {
            lcd.print("Opened");
          }
          else
          {
            lcd.print("Closed");
          }
          break;
  #endif // SERVO_MODULE

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
          break;
  #endif

  #ifdef BMP280_MODULE
        case LCD_SCREEN_BMP280:
          lcd.clear();
          lcd.print("Pres.: ");
          lcd.print(bmp280.getPressure());
          lcd.print(" atm");
          lcd.setCursor(0, 1);
          lcd.print("Alt.: ");
          lcd.print(bmp280.getAltitude());
          lcd.print(" m");
          break;
  #endif

  #ifdef LIGHT_SENSOR_MODULE
        case LCD_SCREEN_LIGHT:
          lcd.clear();
          lcd.print("Light level: ");
          lcd.print(lightSensor.getLightValuePercentage());
          lcd.progressBar(1, lightSensor.getLightValuePercentage());
          break;
  #endif

  #ifdef ULTRASONIC_MODULE
        case LCD_SCREEN_ULTRASONIC:
          lcd.clear();
          lcd.print("Distance: ");
          lcd.print(ultrasonic.getDistance('c'));
          lcd.print(" cm");
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

  #ifdef HUSKYLENS_MODULE
        case LCD_SCREEN_CAMERA_FACE_DETECTED:
          lcd.clear();
          lcd.print("Face Detected");
          lcd.setCursor(0, 1);
          lcd.print("ID: ");
          _result = huskylens.getResult();
          lcd.print(_result.ID);
          break;

        case LCD_SCREEN_CAMERA_NONE:
          lcd.clear();
          lcd.print("Nothing");
          break;
  #endif // HUSKYLENS_MODULE

        default:
          lcd.clear();
          lcd.print("Blank screen");
          break;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(DELAY_LCD));
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