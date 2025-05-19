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
#include "ArduinoJson.h"
#include "globals.h"
#include <DNSServer.h>
#include <FS.h>
#include <LittleFS.h>
#include <WebServer.h>
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
const char *_ssid     = UNI_SSID;
const char *_username = UNI_USERNAME;
const char *_password = UNI_PASSWORD;
#endif

WebServer server(80);
DNSServer dnsServer;

const byte DNS_PORT = 53;

/* Task definitions ------------------------------------------- */
void handleSave()
{
  Preferences prefs;
  if (server.method() != HTTP_POST)
  {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String               body = server.arg("plain");
  DynamicJsonDocument  doc(512);
  DeserializationError error = deserializeJson(doc, body);
  if (error)
  {
    server.send(400, "text/plain", "Bad Request");
    return;
  }

  prefs.begin("config", false);
  prefs.putString("ssid", String(doc["ssid"].as<const char *>()));
  prefs.putString("pass", String(doc["password"].as<const char *>()));
  prefs.putString("mqttBroker", String(doc["mqttBroker"].as<const char *>()));
  prefs.putString("mqttUser", String(doc["mqttUsername"].as<const char *>()));
  prefs.putString("mqttPass", String(doc["mqttPassword"].as<const char *>()));
  prefs.putString("mqttToken", String(doc["mqttToken"].as<const char *>()));
  prefs.end();

  server.send(200, "application/json", "{\"status\":\"ok\"}");
  delay(5000); // Wait 5s before restart
  esp_restart();
}

void captivePortalTask(void *pvParameters)
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP("XIAO-SMART-HOME", "xiao1234");

  if (!LittleFS.begin())
  {
#ifdef DEBUG_PRINT
    Serial.println("LittleFS mount failed");
#endif
    vTaskDelete(NULL);
  }

  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/save", HTTP_POST, handleSave);

  // Captive portal OS-specific paths
  server.on("/generate_204", HTTP_GET, []() {
    Serial.println("generate_204");
    server.send(200, "text/html", "<meta http-equiv='refresh' content='0; url=/' />");
  });

  server.on("/hotspot-detect.html", HTTP_GET, []() {
    Serial.println("hotspot-detect.html");
    server.send(200, "text/html", "<meta http-equiv='refresh' content='0; url=/' />");
  });

  server.on("/connecttest.txt", HTTP_GET, []() { server.send(200, "text/plain", "XIAO_SMART_HOME"); });

  server.on("/ncsi.txt", HTTP_GET, []() { server.send(200, "text/plain", "Microsoft NCSI"); });

  server.serveStatic("/", LittleFS, "/");

  // Catch-all handler for anything else
  server.onNotFound([]() {
    if (LittleFS.exists("/index.html"))
    {
      File file = LittleFS.open("/index.html", "r");
      server.streamFile(file, "text/html");
      file.close();
    }
    else
    {
      server.send(404, "text/plain", "File not found");
    }
  });

  server.begin();

#ifdef DEBUG_PRINT
  Serial.println("HTTP server started in AP mode");
#endif

  for (;;)
  {
    dnsServer.processNextRequest();
    server.handleClient();
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void wifiTask(void *pvParameters)
{
  Preferences prefs;
  if (prefs.begin("config", true))
  {
    ssid     = prefs.getString("ssid", "");
    password = prefs.getString("pass", "");
    prefs.end();
  }
  else
  {
#ifdef DEBUG_PRINT
    Serial.println("Failed to open preferences storage");
#endif
    ssid     = "";
    password = "";
  }

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
      WiFi.begin(bleSsid, blePassword);
#else
      WiFi.begin(ssid.c_str(), password.c_str());
#endif // defined(ENTERPRISE_WIFI)
      // Set a timeout (10 seconds)
      unsigned long startTime = millis();
      while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000)
      {
        vTaskDelay(pdMS_TO_TICKS(500));
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
        if (wifiConnected)
        {
#ifdef DEBUG_PRINT
          Serial.println("WiFi failed. Retrying...");
#endif // DEBUG_PRINT

#ifdef LCD_MODULE
          lcd.clear();
          lcd.print("WiFi Failed");
          lcd.setCursor(0, 1);
          lcd.print("Retrying...");
#endif // LCD_MODULE
        }
        else
        {
#ifdef DEBUG_PRINT
          Serial.println("WiFi failed. Starting AP mode.");
#endif // DEBUG_PRINT
          xTaskCreate(captivePortalTask, "AP Server Task", 8192, NULL, 1, NULL);
#ifdef LCD_MODULE
          lcd.clear();
          lcd.print("Start AP");
          lcd.setCursor(0, 1);
          lcd.print("192.168.4.1");
#endif // LCD_MODULE

          vTaskDelete(NULL);
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10000)); // Check every 10 seconds
  }
}

void wifiSetup()
{
  xTaskCreatePinnedToCore(wifiTask, "WiFi Task", 4096, NULL, 1, NULL, CONFIG_ARDUINO_RUNNING_CORE);
}

/* End of file -------------------------------------------------------- */