/**
 * @file       wifi_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for WiFi Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "wifi_task.h"
#include "globals.h"
#include <LCD-I2C.h>
#include <WiFi.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions ------------------------------------------- */
void wifiTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, password);

      // Set a timeout (10 seconds)
      unsigned long startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
      {
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }

      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("Connected to WiFi");
        lcd.clear();
        lcd.print("WiFi connected");
        lcd.setCursor(0, 1);
        lcd.print("IP: ");
        lcd.print(WiFi.localIP());
      }
      else
      {
        Serial.println("Failed to connect to WiFi. Retrying...");
        lcd.clear();
        lcd.print("WiFi Failed");
        lcd.setCursor(0, 1);
        lcd.print("Check SSID/PW");
      }
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS); // Retry after 10 seconds
  }
}

void wifiSetup() { xTaskCreate(wifiTask, "WiFiTask", 4096, NULL, 1, NULL); }

/* End of file -------------------------------------------------------- */