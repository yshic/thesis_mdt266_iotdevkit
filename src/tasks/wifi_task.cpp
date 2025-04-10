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

#ifdef ENTERPRISE_WIFI
  #include <esp_wpa2.h>
#endif

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
#ifdef ENTERPRISE_WIFI
const char *_ssid     = "HCMUT01";
const char *_username = "";
const char *_password = "";
#endif

/* Task definitions ------------------------------------------- */
void wifiTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      WiFi.mode(WIFI_STA);
#if defined(ENTERPRISE_WIFI)
      // WPA2-Enterprise Config
      esp_wifi_sta_wpa2_ent_set_identity((uint8_t *) _username, strlen(_username));
      esp_wifi_sta_wpa2_ent_set_username((uint8_t *) _username, strlen(_username));
      esp_wifi_sta_wpa2_ent_set_password((uint8_t *) _password, strlen(_password));
      esp_wifi_sta_wpa2_ent_enable();
      WiFi.begin(_ssid);
#elif defined(BLE_MODULE)
      WiFi.begin(ssid, password);
#else
      WiFi.begin("Chi Huong", "nlhtnlat");
#endif // defined(ENTERPRISE_WIFI)
      // Set a timeout (10 seconds)
      unsigned long startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
      {
        vTaskDelay(500 / portTICK_PERIOD_MS);
      }

      if (WiFi.status() == WL_CONNECTED)
      {
        wifiConnected = true;
#ifdef DEBUG_PRINT
        Serial.println("Connected to WiFi");
#endif // DEBUG_PRINT

#ifdef LCD_MODULE
        lcd.clear();
        lcd.print("WiFi connected");
        lcd.setCursor(0, 1);
        lcd.print("IP: ");
        lcd.print(WiFi.localIP());
#endif // LCD_MODULE
      }
      else
      {
        wifiConnected = false;
#ifdef DEBUG_PRINT
        Serial.println("Failed to connect to WiFi. Retrying...");
#endif // DEBUG_PRINT

#ifdef LCD_MODULE
        lcd.clear();
        lcd.print("WiFi Failed");
        lcd.setCursor(0, 1);
        lcd.print("Check SSID/PW");
#endif // LCD_MODULE
      }
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS); // Retry after 10 seconds
  }
}

void wifiSetup()
{
  xTaskCreatePinnedToCore(wifiTask, "WiFiTask", 4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
}

/* End of file -------------------------------------------------------- */