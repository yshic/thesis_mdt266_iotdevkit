/* Includes ----------------------------------------------------------- */
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <WiFi.h>
#include <Wire.h>
#include <string.h>

/**** Custom libraries ****/
// Display
#include "LCD-I2C.h"

// Sensors
#include "dht20.h"
#include "light_sensor.h"
#include "pir_sensor.h"
#include "soil_moisture.h"
#include "ultrasonic.h"

// Motor
#include "mini_fan.h"

// Misc
#include "TaskScheduler.h"
#include "button.h"
#include "relay.h"
#include "usb_switch.h"
#include "utility.h"

// Connectivity
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/* Public defines ----------------------------------------------------- */
// DEBUGGING
// #define DEBUG_I2C

// Main code
#ifndef DEBUG_I2C
  #define MAIN

  // Scheduler
  #define USE_SCHEDULER

  // Connectivity
  // #define BLE_ENABLE
  #define WIFI_ENABLE
  #define MQTT_ENABLE

  // Display
  #define LCD_MODULE

  // Sensors
  #define DHT20_MODULE
  #define LIGHT_MODULE
  #define ULTRASONIC_MODULE
  #define PIR_MODULE
  #define MOISTURE_MODULE

// Actuators
  #define MINI_FAN_MODULE
// #define RELAY_MODULE
// #define USB_SWITCH_MODULE

  #define NUMBER_OF_FEEDS 10

// Input
  #define BUTTON_MODULE

  #define AIO_SERVER     "io.adafruit.com"
  #define AIO_SERVERPORT 1883
  #define AIO_USERNAME   "yshic"
  #define AIO_KEY        "aio_mtOJ25rUqKW2h1s159boohMbqWmF"
#endif

/* Forward function declaration --------------------------------------- */
#ifdef MAIN
// Display
void lcdUpdate();
void lcdScreenStateUpdate(bool increment);

// Connectivity
void bleSetup();
void checkBLECredentials();
void wifiConnect();
void wifiCheck();
void mqttConnect();
void mqttCheck();
void mqttCheckSubscription();

// Sensors
void dht20Update();
void lightSensorUpdate();
void ultrasonicUpdate();
void soilMoistureUpdate();
void pirSensorUpdate();

// Actuators
void miniFanUpdate();
void relayUpdate();
void usbSwitchUpdate();

// Input
void buttonUpdate();
#endif

/* Public variables/object --------------------------------------------------- */
#ifdef MAIN
  // Objects
  #ifdef USE_SCHEDULER
Scheduler ts;
  #endif

  #ifdef LCD_MODULE
LCD_I2C lcd(0x21, 16, 2);

typedef enum
{
  LCD_SCREEN_DHT20,
  LCD_SCREEN_LIGHT,
  LCD_SCREEN_ULTRASONIC,
  LCD_SCREEN_MOISTURE,
  LCD_SCREEN_PIR,
  LCD_SCREEN_MINIFAN,
  LCD_SCREEN_PUMPING,
  LCD_SCREEN_COUNT // Total number of states
} lcd_screen_state_t;
  #endif

  #ifdef DHT20_MODULE
DHT20 dht20;
  #endif

  #ifdef LIGHT_MODULE
LightSensor lightSensor(A9);
  #endif

  #ifdef ULTRASONIC_MODULE
Ultrasonic ultrasonic(D1, D2);
  #endif

  #ifdef MOISTURE_MODULE
SoilMoisture soilMoisture(A0);
  #endif

  #ifdef PIR_MODULE
PIRSensor pirSensor(D7);
  #endif

  #ifdef MINI_FAN_MODULE
MiniFan miniFan(A8);
  #endif

  #ifdef RELAY_MODULE
Relay relay(D8);
  #endif

  #ifdef USB_SWITCH_MODULE
UsbSwitch usbSwitch(D7, D6);
  #endif

  #ifdef BUTTON_MODULE
ButtonHandler button(D2, false, true);
  #endif

WiFiClient client;

Adafruit_MQTT_Client     mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe *subscription;

Adafruit_MQTT_Publish   mqttTemperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish   mqttHumidity    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Subscribe mqttFanControl  = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/fan-control");

// Variables
lcd_screen_state_t lcdScreenState      = LCD_SCREEN_DHT20;
bool               wifiFirstConnection = true;
bool               mqttFirstConnection = true;

// BLE
BLECharacteristic *ssidCharacteristic;
BLECharacteristic *passwordCharacteristic;

  // Wi-Fi Credentials
  #ifdef BLE_ENABLE
char ssid[32]     = "";
char password[32] = "";
  #else
char ssid[32]     = "Chi Huong";
char password[32] = "nlhtnlat";
  #endif

bool wifiCredentialsReady = false;

  // Tasks
  #ifdef USE_SCHEDULER
    #ifdef BLE_ENABLE
Task tCheckBLECredentials(1000, TASK_FOREVER, &checkBLECredentials);
    #endif
Task tWifiConnect(TASK_IMMEDIATE, TASK_ONCE, &wifiConnect);   // Run once at start
Task tMqttConnect(TASK_IMMEDIATE, TASK_ONCE, &mqttConnect);   // Run once after established WiFi connection
Task tLcd(500, TASK_FOREVER, &lcdUpdate);                     // Update every 0.5s
Task tButton(10, TASK_FOREVER, &buttonUpdate);                // Update every 0.01s
Task tDht(30000, TASK_FOREVER, &dht20Update);                 // Update every 30s
Task tLightSensor(1000, TASK_FOREVER, &lightSensorUpdate);    // Update every 1s
Task tUltrasonic(1000, TASK_FOREVER, &ultrasonicUpdate);      // Update every 1s
Task tSoilMoisture(30000, TASK_FOREVER, &soilMoistureUpdate); // Update every 30s
Task tPirSensor(500, TASK_FOREVER, &pirSensorUpdate);         // Update every 0.5s
Task tMiniFan(500, TASK_FOREVER, &miniFanUpdate);             // Update every 0.5s
Task tWifiCheck(10000, TASK_FOREVER, &wifiCheck);             // Update every 10s
Task tMqttCheck(5000, TASK_FOREVER, &mqttCheck);              // Update every 5s
Task tCheckSubscription(100, TASK_FOREVER, &mqttCheckSubscription); // Update every 0.1s
  #endif
#endif

/* Public Class/Struct     --------------------------------------------------- */
class SSIDCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic) override
  {
    std::string value = characteristic->getValue();
    if (value.length() < sizeof(ssid))
    {
      memset(ssid, 0, sizeof(ssid));
      memcpy(ssid, value.c_str(), value.length());
    }
  }
};

class PasswordCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic) override
  {
    std::string value = characteristic->getValue();
    if (value.length() < sizeof(password))
    {
      memset(password, 0, sizeof(password));
      memcpy(password, value.c_str(), value.length());
    }
  }
};

/* Function definitions --------------------------------------- */
#ifdef BLE_ENABLE
void bleSetup()
{
  BLEDevice::init("XIAO ESP32-S3");
  BLEServer  *pServer  = BLEDevice::createServer();
  BLEService *pService = pServer->createService("c3251bfb-5e94-4a01-93e7-6cb22b839cac");

  ssidCharacteristic =
  pService->createCharacteristic("b6c3dda3-c00b-4fa0-a34f-ca91de83a2e4", BLECharacteristic::PROPERTY_WRITE);
  passwordCharacteristic =
  pService->createCharacteristic("8f938de4-56db-45df-9333-912e290efed9", BLECharacteristic::PROPERTY_WRITE);

  // Attach callbacks
  ssidCharacteristic->setCallbacks(new SSIDCallback());
  passwordCharacteristic->setCallbacks(new PasswordCallback());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("c3251bfb-5e94-4a01-93e7-6cb22b839cac");
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void checkBLECredentials()
{
  // Check if BLE has provided valid credentials
  if (strlen(ssid) > 0 && strlen(password) > 0 && !wifiCredentialsReady)
  {
    WiFi.begin(ssid, password);
    unsigned long       startAttemptTime      = millis();
    const unsigned long wifiValidationTimeout = 5000; // 5 seconds timeout for validation

    // Try connecting for a short period
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(100); // Allow system tasks to run
      if (millis() - startAttemptTime >= wifiValidationTimeout)
      {
        // Clear the invalid credentials and notify user
        memset(ssid, 0, sizeof(ssid));
        memset(password, 0, sizeof(password));
        return;
      }
    }

    // Wi-Fi credentials validated
    WiFi.disconnect(); // Disconnect for main Wi-Fi connection later
    wifiCredentialsReady = true;

    // Proceed with Wi-Fi connection
    ts.addTask(tWifiConnect);
    tWifiConnect.enable();
    tCheckBLECredentials.disable();
    ts.deleteTask(tCheckBLECredentials);
  }
}
#endif

#ifdef WIFI_ENABLE
void wifiConnect()
{
  WiFi.begin(ssid, password);
  lcd.clear();
  lcd.print("Connecting...");

  // Wait for the WiFi event
  while (WiFi.status() != WL_CONNECTED)
  {
  }
  lcd.clear();
  lcd.print("WiFi connected");
  lcd.setCursor(0, 1);
  lcd.print("IP: ");
  lcd.print(WiFi.localIP());
  delay(2000);

  // Connect to MQTT after WiFi connection is established
  if (wifiFirstConnection)
  {
    // Add task to connect to MQTT
    ts.addTask(tMqttConnect);
    tMqttConnect.enable();
  }
  wifiFirstConnection = false;
}

void wifiCheck()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    lcd.clear();
    lcd.print("WIFI LOST!");
    lcd.setCursor(0, 1);
    lcd.print("RECONNECTING...");
    delay(2000);
    wifiConnect();
  }
}
#endif

#ifdef MQTT_ENABLE
void mqttConnect()
{
  mqtt.connect();
  lcd.clear();
  lcd.print("Connecting MQTT");
  int8_t returnValue;
  if (mqtt.connected())
  {
    lcd.clear();
    lcd.print("MQTT Connected");
    delay(1000);

    // Add main task after MQTT connection established
    if (mqttFirstConnection)
    {
      ts.addTask(tWifiCheck);
      tWifiCheck.enable();

      ts.addTask(tMqttCheck);
      tMqttCheck.enable();

      ts.addTask(tCheckSubscription);
      tCheckSubscription.enable();

      ts.addTask(tLcd);
      tLcd.enable();

      ts.addTask(tButton);
      tButton.enable();

      ts.addTask(tDht);
      tDht.enable();

      ts.addTask(tLightSensor);
      tLightSensor.enable();

      ts.addTask(tUltrasonic);
      tUltrasonic.enable();

      ts.addTask(tSoilMoisture);
      tSoilMoisture.enable();

      ts.addTask(tMiniFan);
      tMiniFan.enable();

      ts.addTask(tPirSensor);
      tPirSensor.enable();
    }
    mqttFirstConnection = false;
    return;
  }

  uint8_t retries = 3;
  while ((returnValue = mqtt.connect()) != 0)
  {
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0)
    {
      return;
    }
  }
}

void mqttCheck()
{
  if (mqtt.connected() == false)
  {
    mqttConnect();
  }
}

void mqttCheckSubscription()
{
  /*if (subscription == &lightbutton)
  {
    if (strcmp((char *) lightbutton.lastread, "1") == 0)
    {
      NeoPixel.clear();
      NeoPixel.show();
    }
    if (strcmp((char *) onoffbutton.lastread, "0") == 0)
    {
      for (int pixel = 0; pixel < NUM_PIXELS; pixel++)
      {
        NeoPixel.setPixelColor(pixel, NeoPixel.Color(255, 0, 0));
      }
      NeoPixel.show();
    }
  }
  */
  subscription = mqtt.readSubscription();
  if (subscription == &mqttFanControl)
  {
    char *value  = (char *) mqttFanControl.lastread;
    int   _value = atoi(value);
    miniFan.setFanSpeedPercentage(_value);
  }
}
#endif

#ifdef LCD_MODULE
void lcdUpdate()
{
  switch (lcdScreenState)
  {
    case LCD_SCREEN_DHT20:
  #ifdef DHT20_MODULE
      lcd.clear();
      lcd.print("Hum: ");
      lcd.print(dht20.getHumidity());
      lcd.print(" %");
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(dht20.getTemperature());
      lcd.print(" *C");
  #endif
      break;
    case LCD_SCREEN_LIGHT:
  #ifdef LIGHT_MODULE
      lcd.clear();
      lcd.print("Light level: ");
      lcd.print(lightSensor.getLightValuePercentage());
      lcd.progressBar(1, lightSensor.getLightValuePercentage());
  #endif
      break;
    case LCD_SCREEN_ULTRASONIC:
  #ifdef ULTRASONIC_MODULE
      lcd.clear();
      lcd.print("Distance: ");
      lcd.print(ultrasonic.getDistance('c'));
      lcd.print(" cm");
  #endif
      break;
    case LCD_SCREEN_MOISTURE:
  #ifdef MOISTURE_MODULE
      lcd.clear();
      lcd.print("Moisture: ");
      lcd.print(soilMoisture.getMoisture());
      lcd.setCursor(0, 1);
      lcd.print(soilMoisture.getMoisturePercentage());
      lcd.print(" %");
  #endif
      break;
    case LCD_SCREEN_PIR:
  #ifdef PIR_MODULE
      lcd.clear();
      lcd.print(pirSensor.getStatus() ? "Motion Detected!" : "No Motion!");
  #endif
      break;
    case LCD_SCREEN_MINIFAN:
  #ifdef MINI_FAN_MODULE
      lcd.clear();
      lcd.print("Fan Speed: ");
      lcd.print(miniFan.getFanSpeed());
      lcd.setCursor(0, 1);
      lcd.print(miniFan.getFanSpeedPercentage());
  #endif
      break;
    default:
      lcd.clear();
      break;
  }
}

void lcdScreenStateUpdate(bool increment)
{
  if (increment)
  {
    lcdScreenState = (lcd_screen_state_t) ((lcdScreenState + 1) % LCD_SCREEN_COUNT);
  }
  else
  {
    lcdScreenState = (lcd_screen_state_t) ((lcdScreenState - 1 + LCD_SCREEN_COUNT) % LCD_SCREEN_COUNT);
  }
}
#endif

#ifdef DHT20_MODULE
void dht20Update()
{
  dht20.readTempAndHumidity();
  mqttHumidity.publish(dht20.getHumidity());
  mqttTemperature.publish(dht20.getTemperature());
}
#endif

#ifdef LIGHT_MODULE
void lightSensorUpdate() { lightSensor.read(); }
#endif

#ifdef ULTRASONIC_MODULE
void ultrasonicUpdate()
{
  ultrasonic.read('c');
  ultrasonic.read('m');
  ultrasonic.read('i');
}
#endif

#ifdef MOISTURE_MODULE
void soilMoistureUpdate() { soilMoisture.read(); }
#endif

#ifdef PIR_MODULE
void pirSensorUpdate()
{
  pirSensor.read();
  digitalWrite(LED_BUILTIN, pirSensor.getStatus() ? LOW : HIGH);
}
#endif

#ifdef MINI_FAN_MODULE
void miniFanUpdate() { miniFan.getFanSpeed(); }
#endif

#ifdef RELAY_MODULE
void relayUpdate() {}
#endif

#ifdef USB_SWITCH_MODULE
void usbSwitchUpdate() {}
#endif

#ifdef BUTTON_MODULE
void buttonUpdate() { button.update(); }
#endif

/* Main code ----------------------------------------------------- */
void setup()
{
  Serial.begin(9600);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

#ifdef MAIN
  #ifdef USE_SCHEDULER
  ts.init();
    #ifdef BLE_ENABLE
  bleSetup();
  ts.addTask(tCheckBLECredentials);
  tCheckBLECredentials.enable();
    #endif

    #ifndef BLE_ENABLE
      #ifdef WIFI_ENABLE
  ts.addTask(tWifiConnect);
  tWifiConnect.enable();
      #endif
    #endif
  #endif

  #ifdef MQTT_ENABLE
  // Subscribe to topics
  mqtt.subscribe(&mqttFanControl);
  #endif

  #ifdef DHT20_MODULE
  dht20.begin();
  #endif

  #ifdef LCD_MODULE
  lcd.begin(&Wire);
  lcd.display();
  lcd.backlight();
  lcd.clear();
  #endif

  #ifdef PIR_MODULE
  pirSensor.setDebounceTime(50);
  #endif

  #ifdef BUTTON_MODULE
  // Button timing config
  button.setDebounceDuration(50);
  button.setDoubleClickInterval(300);
  button.setHoldDuration(1000);

  // Setup callback
  button.attachSingleClickCallback([]() { lcdScreenStateUpdate(true); });
  button.attachDoubleClickCallback([]() { lcdScreenStateUpdate(false); });
  #endif
#endif

#ifdef DEBUG_I2C
  while (!Serial)
    ; // Wait for Serial to be ready
  scanI2CDevices();
#endif
}

void loop()
{
#ifdef DEBUG_I2C
  scanI2CDevices();
#endif

#ifdef MAIN
  #ifdef USE_SCHEDULER
  ts.execute();
  #endif
#endif
}