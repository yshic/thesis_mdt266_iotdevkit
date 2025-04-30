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
#include <WiFiManager.h>

#ifdef ENTERPRISE_WIFI
  #include <esp_wpa2.h>
#endif

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
#ifdef ENTERPRISE_WIFI
const char *_ssid     = UNI_SSID;
const char *_username = UNI_USERNAME;
const char *_password = UNI_PASSWORD;
#endif

#ifdef USE_CAPTIVE_PORTAL
WiFiManager wm;
bool        portalActive = false;
#endif // USE_CAPTIVE_PORTAL

/* Task definitions ------------------------------------------- */
void wifiTask(void *pvParameters)
{
#ifdef USE_CAPTIVE_PORTAL
  static bool wasConnected = wifiConnected;
#endif // USE_CAPTIVE_PORTAL

  for (;;)
  {
#ifdef USE_CAPTIVE_PORTAL
    if (WiFi.status() != WL_CONNECTED)
    {
      if (wasConnected)
      {
  #ifdef DEBUG_PRINT
        Serial.println("[WiFiTask] Lost WiFi. Attempting reconnect...");
  #endif
        // Retry WiFiManager autoConnect
        if (wm.autoConnect("XIAO-Setup", "xiao1234"))
        {
  #ifdef DEBUG_PRINT
          Serial.println("[WiFiTask] Reconnected to WiFi!");
          Serial.print("SSID: ");
          Serial.println(WiFi.SSID());
          Serial.print("IP Address: ");
          Serial.println(WiFi.localIP());
  #endif
          wifiConnected = true;
          portalActive  = false;
        }
        else
        {
  #ifdef DEBUG_PRINT
          Serial.println("[WiFiTask] Failed to reconnect. Config portal active.");
  #endif
          wifiConnected = false;
          portalActive  = true;
        }

        // Avoid spamming reconnects
        wasConnected = false;
      }
    }
    else
    {
      if (!wasConnected)
      {
        wasConnected = true;
  #ifdef DEBUG_PRINT
        Serial.println("[WiFiTask] WiFi is back!");
        wifiConnected = true;
  #endif
      }
    }
#else
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
      WiFi.begin(DEFAULT_SSID_HOME, DEFAULT_PASSWORD_HOME);
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
#endif   // USE_CAPTIVE_PORTAL

    vTaskDelay(pdMS_TO_TICKS(10000)); // Check every 10 seconds
  }
}

#ifdef USE_CAPTIVE_PORTAL
void wmProcessTask(void *pvParameters)
{
  for (;;)
  {
    // Only call wm.process() if the portal is actually active
    if (portalActive && wm.getConfigPortalActive())
    {
      wm.process();
    }

    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
#endif // USE_CAPTIVE_PORTAL

void wifiSetup()
{
#ifdef USE_CAPTIVE_PORTAL
  WiFi.mode(WIFI_STA);

  // Configure WiFiManager
  button.attachHoldReleaseCallback([]() {
  #ifdef DEBUG_BUTTON_CALLBACK
    Serial.println("Hold Release");
  #endif                // DEBUG_BUTTON_CALLBACK
    wm.resetSettings(); // wipe credentials on button hold
    esp_restart();
  });
  // wm.setWpa2Mode(true);              // use wpa2 if needed
  // wm.addParameter(&custom_username); // username for wpa2
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(180); // 180 seconds timeout
  wm.setCaptivePortalEnable(true);
  wm.setCustomHeadElement("<div style='text-align:center; margin:20px;'>"
                          "<img src='' style='width:150px;'>"
                          "</div>");

  if (wm.autoConnect("XIAO-Setup", "xiao1234"))
  {
  #ifdef DEBUG_PRINT
    Serial.println("Connected to WiFi!");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  #endif // DEBUG_PRINT

    wifiConnected = true;
    portalActive  = false;
  }
  else
  {
  #ifdef DEBUG_PRINT
    Serial.println("Entered config mode");
    Serial.println("Connect to AP: " + wm.getConfigPortalSSID());
    Serial.println("Portal URL: http://192.168.4.1");
  #endif // DEBUG_PRINT

    wifiConnected = false;
    portalActive  = true;
    xTaskCreate(&wmProcessTask, "WM Process Task", 4096, NULL, 1, NULL);
  }
#endif // USE_CAPTIVE_PORTAL
  xTaskCreatePinnedToCore(wifiTask, "WiFi Task", 4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
}

/* End of file -------------------------------------------------------- */