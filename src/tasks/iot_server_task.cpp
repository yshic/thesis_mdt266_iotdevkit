/**
 * @file       iot_server_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for IOT Server Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "iot_server_task.h"
#include "bsp_gpio.h"
#include "globals.h"

#include <Arduino_MQTT_Client.h>
#include <WiFi.h>

#include <Attribute_Request.h>
#include <Server_Side_RPC.h>
#include <Shared_Attribute_Update.h>
#include <ThingsBoard.h>

#include <Espressif_Updater.h>
#include <OTA_Firmware_Update.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

// Firmware Title & Version
constexpr char CURRENT_FIRMWARE_TITLE[]   = "XIAO_SMART_HOME";
constexpr char CURRENT_FIRMWARE_VERSION[] = "0.5.0";

// Maximum amount of retries we attempt to download each firmware chunck over MQTT
constexpr uint8_t FIRMWARE_FAILURE_RETRIES = 12U;

// Size of each firmware chunck downloaded over MQTT,
// increased packet size, might increase download speed
constexpr uint16_t FIRMWARE_PACKET_SIZE = 4096U;

constexpr char     TOKEN[]          = "nn0o282f0s9xx6m6ujp6";
constexpr char     COREIOT_SERVER[] = "app.coreiot.io";
constexpr uint16_t COREIOT_PORT     = 1883U;

// Maximum size packets will ever be sent or received by the underlying MQTT client,
// if the size is to small messages might not be sent or received messages will be discarded
constexpr uint16_t MAX_MESSAGE_SEND_SIZE    = 512U;
constexpr uint16_t MAX_MESSAGE_RECEIVE_SIZE = 512U;
constexpr size_t   MAX_ATTRIBUTES           = 15U;

constexpr const char RPC_SWITCH_METHOD[]                = "setSwitchState";
constexpr char       RPC_REQUEST_CALLBACK_METHOD_NAME[] = "getCurrentTime";
constexpr uint8_t    MAX_RPC_SUBSCRIPTIONS              = 10U;
constexpr uint8_t    MAX_RPC_REQUEST                    = 10U;
constexpr uint64_t   REQUEST_TIMEOUT_MICROSECONDS       = 15000U * 1000U;

constexpr int16_t telemetrySendInterval = 10000U;

constexpr char TEMPERATURE_KEY[]      = "temperature";
constexpr char HUMIDITY_KEY[]         = "humidity";
constexpr char ILLUMINANCE_KEY[]      = "illuminance";
constexpr char PRESSURE_KEY[]         = "pressure";
constexpr char ALTITUDE_KEY[]         = "altitude";
constexpr char VOLTAGE_KEY[]          = "voltage";
constexpr char CURRENT_KEY[]          = "current";
constexpr char POWER_KEY[]            = "power";
constexpr char POWER_FACTOR_KEY[]     = "powerFactor";
constexpr char POWER_EFFICIENCY_KEY[] = "powerEfficiency";

// Attribute names
constexpr char LED_STATE_ATTR[]    = "ledState";
constexpr char FAN_SPEED_ATTR[]    = "fanSpeed";
constexpr char FW_TITLE_ATTR[]     = "fw_title";
constexpr char FW_VERSION_ATTR[]   = "fw_version";
constexpr char RELAY1_STATE_ATTR[] = "relay1State";
constexpr char RELAY2_STATE_ATTR[] = "relay2State";
constexpr char RELAY3_STATE_ATTR[] = "relay3State";
constexpr char RELAY4_STATE_ATTR[] = "relay4State";

// Flag to handle devices state and values
volatile bool ledStateChanged    = false;
volatile bool fanSpeedChanged    = false;
volatile bool relaysStateChanged = false;

// Current devices states/values
volatile bool ledState       = false;
volatile int  fanSpeed       = 0;
volatile bool relaysState[4] = {false};

// Statuses for updating
bool currentFWSent     = false;
bool updateRequestSent = false;

// Initialize used APIs
OTA_Firmware_Update<>                                   ota;
Server_Side_RPC<MAX_RPC_SUBSCRIPTIONS, MAX_RPC_REQUEST> rpc;
Attribute_Request<2U, MAX_ATTRIBUTES>                   attr_request;
Shared_Attribute_Update<3U, MAX_ATTRIBUTES>             shared_update;

const std::array<IAPI_Implementation *, 4U> apis = {&ota, &rpc, &attr_request, &shared_update};

// List of shared attributes for subscribing to their updates
constexpr std::array<const char *, 7U> SHARED_ATTRIBUTES_LIST = {
FAN_SPEED_ATTR,    FW_TITLE_ATTR,     FW_VERSION_ATTR,  RELAY1_STATE_ATTR,
RELAY2_STATE_ATTR, RELAY3_STATE_ATTR, RELAY4_STATE_ATTR};

// List of client attributes for requesting them (Using to initialize device states)
constexpr std::array<const char *, 1U> CLIENT_ATTRIBUTES_LIST = {LED_STATE_ATTR};

WiFiClient          wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_RECEIVE_SIZE, MAX_MESSAGE_SEND_SIZE, Default_Max_Stack_Size, apis);
Espressif_Updater<> updater;

bool subscribed = false;

/* Private function definitions ------------------------------------------- */
#ifdef OTA_UPDATE_MODULE
void update_starting_callback()
{
  // Nothing to do
}

/// @brief End callback method that will be called as soon as the OTA firmware update, either finished
/// successfully or failed. Is meant to allow to either restart the device if the udpate was successfull or
/// to restart any stopped services before the update started in the subscribed update_starting_callback
/// @param success Either true (update successful) or false (update failed)
void finished_callback(const bool &success)
{
  if (success)
  {
  #ifdef DEBUG_PRINT
    Serial.println("Done, Reboot now");
  #endif // DEBUG_PRINT

  #ifdef ESP32
    esp_restart();
  #endif // ESP32

    return;
  }
  #ifdef DEBUG_PRINT
  Serial.println("Downloading firmware failed");
  #endif // DEBUG_PRINT
}

/// @brief Progress callback method that will be called every time our current progress of downloading the
/// complete firmware data changed, meaning it will be called if the amount of already downloaded chunks
/// increased. Is meant to allow to display a progress bar or print the current progress of the update into
/// the console with the currently already downloaded amount of chunks and the total amount of chunks
/// @param current Already received and processs amount of chunks
/// @param total Total amount of chunks we need to receive and process until the update has completed
void progress_callback(const size_t &current, const size_t &total)
{
  #ifdef DEBUG_PRINT
  Serial.printf("Progress %.2f%%\n", static_cast<float>(current * 100U) / total);
  #endif // DEBUG_PRINT
}
#endif // OTA_UPDATE_MODULE

void processSetSwitchState(const JsonVariantConst &data, JsonDocument &response)
{
  ledState = data;

#ifdef DEBUG_PRINT
  Serial.print("Received set led state RPC. New state: ");
  Serial.println(ledState);
#endif // DEBUG_PRINT

  // Extract the value
  bool newState = data["state"].as<bool>();

#ifdef DEBUG_PRINT
  Serial.print("Switch state change: ");
  Serial.println(newState);
#endif // DEBUG_PRINT

  StaticJsonDocument<1> response_doc;
  // Returning current state as response
  response_doc["newState"] = (int) ledState;
  response.set(response_doc);

  ledStateChanged = true;
}

const std::array<RPC_Callback, 1U> rpcCallbacks = {RPC_Callback{"setLedValue", processSetSwitchState}};

/// @brief Shared attribute update callback
/// @param data New value of shared attributes which is changed
void processSharedAttributes(const JsonObjectConst &data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    const char *key = it->key().c_str();
    // OTA UPDATE
    if (strcmp(key, "fw_title") == 0 || strcmp(key, "fw_version") == 0)
    {
      String fwTitle   = data["fw_title"] | "";
      String fwVersion = data["fw_version"] | "";

      if (fwTitle == CURRENT_FIRMWARE_TITLE && compareVersion(CURRENT_FIRMWARE_VERSION, fwVersion) < 0)
      {
#ifdef DEBUG_PRINT
        Serial.println("New firmware available! Initiating OTA update...");
#endif // DEBUG_PRINT

        const OTA_Update_Callback callback(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION, &updater,
                                           &finished_callback, &progress_callback, &update_starting_callback,
                                           FIRMWARE_FAILURE_RETRIES, FIRMWARE_PACKET_SIZE);
        ota.Start_Firmware_Update(callback);
      }
    }
    // FAN SPEED
    else if (strcmp(key, FAN_SPEED_ATTR) == 0)
    {
      const uint8_t newFanSpeed = it->value().as<uint8_t>();
      if (newFanSpeed <= 100)
      {
        fanSpeed = newFanSpeed;
      }
      else
      {
        fanSpeed = 100;
      }
#ifdef DEBUG_PRINT
      Serial.printf("Fan speed is set to: %d\n", fanSpeed);
#endif // DEBUG_PRINT

      fanSpeedChanged = true;
    }
    // UNIT 4 RELAY
    else if (strcmp(key, RELAY1_STATE_ATTR) == 0)
    {
      relaysState[0]     = it->value().as<bool>();
      relaysStateChanged = true;

#ifdef DEBUG_PRINT
      Serial.printf("Relays updated - 1:%d \n", relaysState[0]);
#endif // DEBUG_PRINT
    }
    else if (strcmp(key, RELAY2_STATE_ATTR) == 0)
    {
      relaysState[1]     = it->value().as<bool>();
      relaysStateChanged = true;

#ifdef DEBUG_PRINT
      Serial.printf("Relays updated - 2:%d \n", relaysState[1]);
#endif // DEBUG_PRINT
    }
    else if (strcmp(key, RELAY3_STATE_ATTR) == 0)
    {
      relaysState[2]     = it->value().as<bool>();
      relaysStateChanged = true;

#ifdef DEBUG_PRINT
      Serial.printf("Relays updated - 3:%d \n", relaysState[2]);
#endif // DEBUG_PRINT
    }
    else if (strcmp(key, RELAY4_STATE_ATTR) == 0)
    {
      relaysState[3]     = it->value().as<bool>();
      relaysStateChanged = true;

#ifdef DEBUG_PRINT
      Serial.printf("Relays updated - 4:%d \n", relaysState[3]);
#endif // DEBUG_PRINT
    }
  }
}

void processClientAttributes(const JsonObjectConst &data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    if (strcmp(it->key().c_str(), LED_STATE_ATTR) == 0)
    {
      ledState = it->value().as<bool>();
      switch ((int) ledState)
      {
        case 0:
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // OFF
          break;
        case 1:
          bspGpioDigitalWrite(LED_BUILTIN, LOW); // ON
          break;
        default:
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // Fail-safe
          break;
      }
    }
  }
}

// Attribute request did not receive a response in the expected amount of microseconds
void requestTimedOut()
{
#ifdef DEBUG_PRINT
  Serial.printf("Attribute request not receive response in (%llu) microseconds. Ensure client is connected "
                "to the MQTT broker and that the keys actually exist on the target device\n",
                REQUEST_TIMEOUT_MICROSECONDS);
#endif // DEBUG_PRINT
}

const Shared_Attribute_Callback<MAX_ATTRIBUTES>
attributes_callback(&processSharedAttributes, SHARED_ATTRIBUTES_LIST.cbegin(), SHARED_ATTRIBUTES_LIST.cend());

const Attribute_Request_Callback<MAX_ATTRIBUTES>
attribute_shared_request_callback(&processSharedAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
                                  SHARED_ATTRIBUTES_LIST);

const Attribute_Request_Callback<MAX_ATTRIBUTES>
attribute_client_request_callback(&processClientAttributes, REQUEST_TIMEOUT_MICROSECONDS, &requestTimedOut,
                                  CLIENT_ATTRIBUTES_LIST);
/* Task definitions ------------------------------------------- */

void iotServerTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED && !tb.connected())
    {
#ifdef DEBUG_PRINT
      Serial.print("Connecting to: ");
      Serial.print(COREIOT_SERVER);
      Serial.print(" with token ");
      Serial.println(TOKEN);
#endif // DEBUG_PRINT

      if (!tb.connect(COREIOT_SERVER, TOKEN, COREIOT_PORT))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to connect");
#endif // DEBUG_PRINT
        vTaskDelay(pdMS_TO_TICKS(1000));
        continue;
      }
      else
      {
#ifdef DEBUG_PRINT
        Serial.println("Connected to IoT server!");
#endif // DEBUG_PRINT
      }

      // Send WiFi attributes
      tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
      tb.sendAttributeData("ssid", WiFi.SSID().c_str());
      tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
      tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());
      tb.sendAttributeData("channel", WiFi.channel());

#ifdef DEBUG_PRINT
      Serial.println("Subscribing for RPC...");
#endif // DEBUG_PRINT
      if (!rpc.RPC_Subscribe(rpcCallbacks.cbegin(), rpcCallbacks.cend()))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to subscribe for RPC");
#endif // DEBUG_PRINT
        return;
      }

      if (!shared_update.Shared_Attributes_Subscribe(attributes_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to subscribe for shared attribute updates");
#endif // DEBUG_PRINT
        return;
      }

#ifdef DEBUG_PRINT
      Serial.println("Subscribe shared attributes done");
#endif // DEBUG_PRINT

      // Request current value of shared attributes
      if (!attr_request.Shared_Attributes_Request(attribute_shared_request_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to request for shared attributes (fan speed)");
#endif // DEBUG_PRINT
        return;
      }

      // Request current states of client attributes
      if (!attr_request.Client_Attributes_Request(attribute_client_request_callback))
      {
#ifdef DEBUG_PRINT
        Serial.println("Failed to request for client attributes (led state)");
#endif // DEBUG_PRINT
        return;
      }

// OTA UPDATE
#ifdef OTA_UPDATE_MODULE
      if (!currentFWSent)
      {
        currentFWSent = ota.Firmware_Send_Info(CURRENT_FIRMWARE_TITLE, CURRENT_FIRMWARE_VERSION);
      }
#endif // OTA_UPDATE_MODULE
    }
    vTaskDelay(pdMS_TO_TICKS(5000)); // Retry after 5 seconds
  }
}

void sendTelemetryTask(void *pvParameters)
{
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;)
  {
    if (tb.connected())
    {
#ifdef DHT20_MODULE
      dht20.readTempAndHumidity();
      float temperature = dht20.getTemperature();
      float humidity    = dht20.getHumidity();
#endif // DHT20_MODULE

#ifdef SHT4X_MODULE
      sht40.update();
      float temperature = sht40.getTemperature();
      float humidity    = sht40.getHumidity();
#endif // SHT4X_MODULE

#ifdef BMP280_MODULE
      bmp280.update();
      float pressure = bmp280.getPressure();
      float altitude = bmp280.getAltitude();
      // float temperature = bmp280.getTemperature();
      if (!(isnan(pressure) || isnan(altitude)))
      {
  #ifdef DEBUG_PRINT
        Serial.print("Pressure: ");
        Serial.print(pressure);
        Serial.print(" Pa, Altitude: ");
        Serial.print(altitude);
        Serial.println(" m");
  #endif // DEBUG_PRINT
        tb.sendTelemetryData(PRESSURE_KEY, pressure);
        tb.sendTelemetryData(ALTITUDE_KEY, altitude);
      }
#endif // BMP280_MODULE

#ifdef AC_MEASURE_MODULE
      if (acMeasure.getReady())
      {
        float   voltage         = acMeasure.getVoltage();
        float   current         = acMeasure.getCurrent();
        float   power           = acMeasure.getPower();
        float   powerFactor     = acMeasure.getPowerFactor();
        uint8_t powerEfficiency = powerFactor * 100;

        if (!(isnan(voltage) || isnan(current) || isnan(power) || isnan(powerFactor)))
        {
  #ifdef DEBUG_PRINT
          Serial.print("Voltage: ");
          Serial.print(voltage);
          Serial.print(" V, Current: ");
          Serial.print(current);
          Serial.println(" A");

          Serial.print("Power: ");
          Serial.print(power);
          Serial.print(" W, Power factor: ");
          Serial.print(powerFactor);
          Serial.println(" %");
  #endif // DEBUG_PRINT

          tb.sendTelemetryData(VOLTAGE_KEY, voltage);
          tb.sendTelemetryData(CURRENT_KEY, current);
          tb.sendTelemetryData(POWER_KEY, power);
          tb.sendTelemetryData(POWER_FACTOR_KEY, powerFactor);
          tb.sendTelemetryData(POWER_EFFICIENCY_KEY, powerEfficiency);
        }
      }
#endif // AC_MEASURE_MODULE

#ifdef LIGHT_SENSOR_MODULE
      lightSensor.read();
      float illuminance = lightSensor.getLightValuePercentage();
      if (!(isnan(illuminance)))
      {
  #ifdef DEBUG_PRINT
        Serial.print("Illuminance: ");
        Serial.print(illuminance);
        Serial.println(" lux");
  #endif // DEBUG_PRINT
        tb.sendTelemetryData(ILLUMINANCE_KEY, illuminance);
      }
#endif // LIGHT_SENSOR_MODULE

#if defined(DHT20_MODULE) || defined(SHT4X_MODULE)
      if (!(isnan(temperature) || isnan(humidity)))
      {
  #ifdef DEBUG_PRINT
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");
  #endif // DEBUG_PRINT
        tb.sendTelemetryData(TEMPERATURE_KEY, temperature);
        tb.sendTelemetryData(HUMIDITY_KEY, humidity);
      }
#endif // defined(DHT20_MODULE) || defined(SHT4X_MODULE)

      // Send WiFi signal strength
      tb.sendAttributeData("rssi", WiFi.RSSI());
    }
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(telemetrySendInterval));
  }
}

void updateDevicesStateTask(void *pvParameters)
{
  for (;;)
  {
    // Update LED
    if (ledStateChanged)
    {
      ledStateChanged = false;

      switch ((int) ledState)
      {
        case 0:
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // OFF
          break;
        case 1:
          bspGpioDigitalWrite(LED_BUILTIN, LOW); // ON
          break;
        default:
          bspGpioDigitalWrite(LED_BUILTIN, HIGH); // Fail-safe
          break;
      }
      tb.sendAttributeData(LED_STATE_ATTR, ledState);
    }

    // Update Fan
    if (fanSpeedChanged)
    {
      fanSpeedChanged = false;

      miniFan.setFanSpeedPercentage(fanSpeed);
    }

    if (relaysStateChanged)
    {
      relaysStateChanged = false;
      for (int i = 0; i < 4; i++)
      {
        if (unit4Relay.getRelayState(i) != relaysState[i])
        {
          unit4Relay.relayWrite(i, relaysState[i]);
        }
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

void thingsboardLoopTask(void *pvParameters)
{
  for (;;)
  {
    tb.loop();
    vTaskDelay(pdMS_TO_TICKS(10)); // Short delay for processing
  }
}

void iotServerSetup()
{
  xTaskCreate(iotServerTask, "IOT Server Task", 8192, NULL, 1, NULL);
  xTaskCreate(sendTelemetryTask, "Send Telemetry Task", 8192, NULL, 1, NULL);
  xTaskCreate(thingsboardLoopTask, "ThingsBoard Loop Task", 8192, NULL, 1, NULL);
  xTaskCreate(updateDevicesStateTask, "Update Devices State Task", 4096, NULL, 1, NULL);
}
/* End of file -------------------------------------------------------- */