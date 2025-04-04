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
#include "iot_server_task.h"
#include "globals.h"
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
#include <Espressif_Updater.h>
>>>>>>> Stashed changes
=======
#include <Espressif_Updater.h>
>>>>>>> Stashed changes
=======
#include <Espressif_Updater.h>
>>>>>>> Stashed changes
=======
#include <Espressif_Updater.h>
>>>>>>> Stashed changes
#include <WiFi.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
constexpr char     TOKEN[]              = "cM3JNEsWAsgFqE7g3O3E";
=======
constexpr char     TOKEN[]              = "8t6sbcmrx76pviaj6nx2";
>>>>>>> Stashed changes
=======
constexpr char     TOKEN[]              = "8t6sbcmrx76pviaj6nx2";
>>>>>>> Stashed changes
=======
constexpr char     TOKEN[]              = "8t6sbcmrx76pviaj6nx2";
>>>>>>> Stashed changes
=======
constexpr char     TOKEN[]              = "8t6sbcmrx76pviaj6nx2";
>>>>>>> Stashed changes
constexpr char     THINGSBOARD_SERVER[] = "app.coreiot.io";
constexpr uint16_t THINGSBOARD_PORT     = 1883U;

constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;

constexpr char LED_STATE_ATTR[]    = "ledState";
constexpr char FAN_SPEED_ATTR[]    = "fanSpeed";
constexpr char RELAY1_STATE_ATTR[] = "relay1State";
constexpr char RELAY2_STATE_ATTR[] = "relay2State";
constexpr char RELAY3_STATE_ATTR[] = "relay3State";
constexpr char RELAY4_STATE_ATTR[] = "relay4State";

volatile bool attributeLedChanged      = false;
volatile int  attributeFanSpeedChanged = 0;
volatile bool ledState                 = false;
volatile int  fanSpeed                 = 0;
volatile bool relay1State              = false;
volatile bool relay2State              = false;
volatile bool relay3State              = false;
volatile bool relay4State              = false;
volatile int  attributeRelay1Changed   = 0;
volatile int  attributeRelay2Changed   = 0;
volatile int  attributeRelay3Changed   = 0;
volatile int  attributeRelay4Changed   = 0;

uint32_t previousStateChange;

constexpr int16_t telemetrySendInterval = 10000U;
uint32_t          previousDataSend;

constexpr std::array<const char *, 6U> SHARED_ATTRIBUTES_LIST = {
LED_STATE_ATTR, FAN_SPEED_ATTR, RELAY1_STATE_ATTR, RELAY2_STATE_ATTR, RELAY3_STATE_ATTR, RELAY4_STATE_ATTR};

WiFiClient          wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard         tb(mqttClient, MAX_MESSAGE_SIZE);

/* Task definitions ------------------------------------------- */
RPC_Response setLedSwitchState(const RPC_Data &data)
{
  Serial.println("Received Switch state");
  bool newState = data;
  Serial.print("Switch state change: ");
  Serial.println(newState);
  switch ((int) newState)
  {
    case 0:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
    case 1:
      digitalWrite(LED_BUILTIN, LOW);
      break;
    default:
      digitalWrite(LED_BUILTIN, HIGH);
      break;
  }
  attributeLedChanged = true;
  return RPC_Response("setLedSwitchValue", newState);
}

RPC_Response setFanSpeed(const RPC_Data &data)
{
  Serial.println("Received Fan Speed");
  int newSpeed = data;
  Serial.print("Fan speed change: ");
  Serial.println(newSpeed);
  miniFan.setFanSpeedPercentage(newSpeed);
  attributeFanSpeedChanged = true;
  return RPC_Response("setFanValue", newSpeed);
}

RPC_Response setRelay1State(const RPC_Data &data)
{
  Serial.println("Received Relay 1 State");
  int newRelay1State = data;
  Serial.print("Relay 1 state change: ");
  Serial.println(newRelay1State);
  unit4Relay.relayWrite(0, newRelay1State);
  return RPC_Response("setRelayState", newRelay1State);
}

RPC_Response setRelay2State(const RPC_Data &data)
{
  Serial.println("Received Relay 2 State");
  int newRelay2State = data;
  Serial.print("Relay 2 state change: ");
  Serial.println(newRelay2State);
  unit4Relay.relayWrite(1, newRelay2State);
  return RPC_Response("setRelayState", newRelay2State);
}

RPC_Response setRelay3State(const RPC_Data &data)
{
  Serial.println("Received Relay 3 State");
  int newRelay3State = data;
  Serial.print("Relay 3 state change: ");
  Serial.println(newRelay3State);
  unit4Relay.relayWrite(2, newRelay3State);
  return RPC_Response("setRelayState", newRelay3State);
}

RPC_Response setRelay4State(const RPC_Data &data)
{
  Serial.println("Received Relay 4 State");
  int newRelay4State = data;
  Serial.print("Relay 4 state change: ");
  Serial.println(newRelay4State);
  unit4Relay.relayWrite(3, newRelay4State);
  return RPC_Response("setRelayState", newRelay4State);
}

const std::array<RPC_Callback, 6U> callbacks = {
RPC_Callback{"setLedSwitchValue", setLedSwitchState}, RPC_Callback{"setFanValue", setFanSpeed},
RPC_Callback{"setRelay1State", setRelay1State},       RPC_Callback{"setRelay2State", setRelay2State},
RPC_Callback{"setRelay3State", setRelay3State},       RPC_Callback{"setRelay4State", setRelay4State}};

void processSharedAttributes(const Shared_Attribute_Data &data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    if (strcmp(it->key().c_str(), LED_STATE_ATTR) == 0)
    {
      ledState = it->value().as<bool>();
      digitalWrite(LED_BUILTIN, ledState);
      Serial.print("LED state is set to: ");
      Serial.println(ledState);
      attributeLedChanged = true;
    }

    // Changed else if -> if
    if (strcmp(it->key().c_str(), FAN_SPEED_ATTR) == 0)
    {
      fanSpeed = it->value().as<int>();
      miniFan.setFanSpeedPercentage(fanSpeed);
      Serial.print("Fan speed is set to: ");
      Serial.println(fanSpeed);
      attributeFanSpeedChanged = true;
    }

    if (strcmp(it->key().c_str(), RELAY1_STATE_ATTR) == 0)
    {
      relay1State = it->value().as<bool>();
      unit4Relay.relayWrite(0, relay1State);
      Serial.print("Relay 1 is set to: ");
      Serial.println(relay1State);
      attributeRelay1Changed = true;
    }

    if (strcmp(it->key().c_str(), RELAY2_STATE_ATTR) == 0)
    {
      relay2State = it->value().as<bool>();
      unit4Relay.relayWrite(1, relay2State);
      Serial.print("Relay 2 is set to: ");
      Serial.println(relay2State);
      attributeRelay2Changed = true;
    }

    if (strcmp(it->key().c_str(), RELAY3_STATE_ATTR) == 0)
    {
      relay3State = it->value().as<bool>();
      unit4Relay.relayWrite(2, relay3State);
      Serial.print("Relay 3 is set to: ");
      Serial.println(relay3State);
      attributeRelay3Changed = true;
    }

    if (strcmp(it->key().c_str(), RELAY4_STATE_ATTR) == 0)
    {
      relay4State = it->value().as<bool>();
      unit4Relay.relayWrite(3, relay4State);
      Serial.print("Relay 4 is set to: ");
      Serial.println(relay4State);
      attributeRelay4Changed = true;
    }
  }
}

const Shared_Attribute_Callback attributes_callback(&processSharedAttributes, SHARED_ATTRIBUTES_LIST.cbegin(),
                                                    SHARED_ATTRIBUTES_LIST.cend());
const Attribute_Request_Callback attribute_shared_request_callback(&processSharedAttributes,
                                                                   SHARED_ATTRIBUTES_LIST.cbegin(),
                                                                   SHARED_ATTRIBUTES_LIST.cend());

void iotServerTask(void *pvParameters)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED && !tb.connected())
    {
      Serial.print("Connecting to: ");
      Serial.print(THINGSBOARD_SERVER);
      Serial.print(" with token ");
      Serial.println(TOKEN);

      if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT))
      {
        Serial.println("Failed to connect");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
      }
      else
      {
        Serial.println("Connected to IoT server!");
      }

      tb.sendAttributeData("macAddress", WiFi.macAddress().c_str());

      // Subscribe for RPC commands and shared attributes
      Serial.println("Subscribing for RPC and shared attributes...");
      if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend()))
      {
        Serial.println("Failed to subscribe for RPC");
      }
      if (!tb.Shared_Attributes_Subscribe(attributes_callback))
      {
        Serial.println("Failed to subscribe for shared attribute updates");
      }
      if (!tb.Shared_Attributes_Request(attribute_shared_request_callback))
      {
        Serial.println("Failed to request shared attributes");
      }
      Serial.println("ThingsBoard connected and subscriptions done");
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS); // Retry after 5 seconds
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

      Serial.print("Pressure: ");
      Serial.print(pressure);
      Serial.print(" Pa, Altitude: ");
      Serial.print(altitude);
      Serial.println(" m");

      tb.sendTelemetryData("pressure", pressure);
      tb.sendTelemetryData("altitude", altitude);
#endif // BMP280_MODULE

#ifdef AC_MEASURE_MODULE
      if (acMeasure.getReady())
      {
        float voltage     = acMeasure.getVoltage();
        float current     = acMeasure.getCurrent();
        float power       = acMeasure.getPower();
        float powerFactor = acMeasure.getPowerFactor();

        Serial.print("Voltage: ");
        Serial.print(voltage);
        Serial.print(" V, Current: ");
        Serial.print(current);
        Serial.println(" A");

        Serial.print("Power: ");
        Serial.print(power);
        Serial.print(" W, Power factor: ");
        Serial.print(powerFactor);
        Serial.println("");

        tb.sendTelemetryData("voltage", voltage);
        tb.sendTelemetryData("current", current);
        tb.sendTelemetryData("power", power);
        tb.sendTelemetryData("powerFactor", powerFactor);
      }
#endif // AC_MEASURE_MODULE

#ifdef LIGHT_SENSOR_MODULE
      lightSensor.read();
      float illuminance = lightSensor.getLightValuePercentage();
      if (!(isnan(illuminance)))
      {
        Serial.print("Illuminance: ");
        Serial.print(illuminance);
        Serial.println(" lux");
        tb.sendTelemetryData("illuminance", illuminance);
      }
#endif // LIGHT_SENSOR_MODULE

#if defined(DHT20_MODULE) || defined(SHT4X_MODULE)
      if (!(isnan(temperature) || isnan(humidity)))
      {
        Serial.print("Temperature: ");
        Serial.print(temperature);
        Serial.print(" °C, Humidity: ");
        Serial.print(humidity);
        Serial.println(" %");

        tb.sendTelemetryData("temperature", temperature);
        tb.sendTelemetryData("humidity", humidity);
      }
#endif // defined(DHT20_MODULE) || defined(SHT4X_MODULE)

      // Send WiFi attributes
      tb.sendAttributeData("rssi", WiFi.RSSI());
      tb.sendAttributeData("channel", WiFi.channel());
      tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str());
      tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str());
      tb.sendAttributeData("ssid", WiFi.SSID().c_str());

      // If any attribute (like LED state) has changed, update ThingsBoard
      if (attributeLedChanged)
      {
        attributeLedChanged = false;
        tb.sendAttributeData(LED_STATE_ATTR, digitalRead(LED_BUILTIN));
      }
      if (attributeFanSpeedChanged)
      {
        attributeFanSpeedChanged = false;
        tb.sendAttributeData(FAN_SPEED_ATTR, miniFan.getFanSpeedPercentage());
      }
      if (attributeRelay1Changed)
      {
        attributeRelay1Changed = false;
        tb.sendAttributeData(RELAY1_STATE_ATTR, unit4Relay.getRelayState(0));
      }

      if (attributeRelay2Changed)
      {
        attributeRelay2Changed = false;
        tb.sendAttributeData(RELAY2_STATE_ATTR, unit4Relay.getRelayState(1));
      }

      if (attributeRelay3Changed)
      {
        attributeRelay3Changed = false;
        tb.sendAttributeData(RELAY3_STATE_ATTR, unit4Relay.getRelayState(2));
      }

      if (attributeRelay4Changed)
      {
        attributeRelay4Changed = false;
        tb.sendAttributeData(RELAY4_STATE_ATTR, unit4Relay.getRelayState(3));
      }
    }
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(telemetrySendInterval));
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
}
/* End of file -------------------------------------------------------- */