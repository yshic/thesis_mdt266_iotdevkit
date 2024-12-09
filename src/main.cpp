/* Includes ----------------------------------------------------------- */
#include <Arduino.h>
#include <Wire.h>

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
// #include "mini_fan.h"

// Misc
#include "OneButton.h"
#include "TaskScheduler.h"
#include "relay.h"
#include "usb_switch.h"
#include "utility.h"

/* Public defines ----------------------------------------------------- */
// DEBUGGING
// #define DEBUG_I2C

// Main code
#ifndef DEBUG_I2C
  #define MAIN
  #define USE_SCHEDULER

  // Display
  #define LCD_MODULE
  #define NUMBER_OF_LCD_SCREEN_STATE 5

  // Sensors
  #define DHT20_MODULE
  #define LIGHT_MODULE
  #define ULTRASONIC_MODULE
  #define PIR_MODULE
  #define MOISTURE_MODULE

// Motor
// #define MINI_FAN_MODULE

// Misc
// #define RELAY_MODULE
// #define USB_SWITCH_MODULE
  #define BUTTON_MODULE
#endif

/* Forward function declaration --------------------------------------- */
void lcdUpdate();
void updateLcdScreenState(bool increment);

void dht20Update();
void lightSensorUpdate();
void ultrasonicUpdate();
void soilMoistureUpdate();
void pirSensorUpdate();

void miniFanUpdate();
void relayUpdate();
void usbSwitchUpdate();
void buttonUpdate();

/* Public variables/object --------------------------------------------------- */
// Objects
#ifdef USE_SCHEDULER
Scheduler ts;
#endif

#ifdef LCD_MODULE
LCD_I2C lcd(0x21, 16, 2);
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
OneButton button;
#endif

// Variables
uint8_t  lcdScreenState          = 0;
float    sensorDHTValues[2]      = {0};
uint16_t sensorLightValue        = 0;
int      sensorMoistureValues[2] = {0};
int16_t  sensorUltrasonicValue   = 0;
uint8_t  sensorPirValue          = 0;

// Tasks
#ifdef USE_SCHEDULER
Task t1Lcd(500, TASK_FOREVER, &lcdUpdate);                     // Update every 0.5s
Task t2Button(10, TASK_FOREVER, &buttonUpdate);                // Update every 0.01s
Task t3Dht(30000, TASK_FOREVER, &dht20Update);                 // Update every 30s
Task t4LightSensor(1000, TASK_FOREVER, &lightSensorUpdate);    // Update every 1s
Task t5Ultrasonic(1000, TASK_FOREVER, &ultrasonicUpdate);      // Update every 1s
Task t6SoilMoisture(30000, TASK_FOREVER, &soilMoistureUpdate); // Update every 30s
Task t7PirSensor(500, TASK_FOREVER, &pirSensorUpdate);         // Update every 0.5s
#endif

/* Function definitions --------------------------------------- */
#ifdef LCD_MODULE
void lcdUpdate()
{
  switch (lcdScreenState)
  {
    case 0:
  #ifdef DHT20_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hum: ");
      lcd.print(sensorDHTValues[0]);
      lcd.print(" %");
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(sensorDHTValues[1]);
      lcd.print(" *C");
  #endif
      break;
    case 1:
  #ifdef LIGHT_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Light level: ");
      lcd.print(sensorLightValue);
      lcd.progressBar(1, sensorLightValue);
  #endif
      break;
    case 2:
  #ifdef ULTRASONIC_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      lcd.print(sensorUltrasonicValue);
      lcd.print(" cm");
  #endif
      break;
    case 3:
  #ifdef MOISTURE_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Moisture: ");
      lcd.print(sensorMoistureValues[0]);
      lcd.setCursor(0, 1);
      lcd.print(sensorMoistureValues[1]);
      lcd.print(" %");
  #endif
      break;
    case 4:
  #ifdef PIR_MODULE
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(sensorPirValue ? "Motion Detected!" : "No Motion!");
  #endif
      break;
    default:
      break;
  }
}

void updateLcdScreenState(bool increment)
{
  if (increment)
  {
    lcdScreenState = (lcdScreenState + 1) % NUMBER_OF_LCD_SCREEN_STATE;
  }
  else
  {
    lcdScreenState = (lcdScreenState > 0) ? lcdScreenState - 1 : NUMBER_OF_LCD_SCREEN_STATE - 1;
  }
}
#endif

#ifdef DHT20_MODULE
void dht20Update() { dht20.readTempAndHumidity(sensorDHTValues); }
#endif

#ifdef LIGHT_MODULE
void lightSensorUpdate() { sensorLightValue = lightSensor.readAndMap(0, 100); }
#endif

#ifdef ULTRASONIC_MODULE
void ultrasonicUpdate() { sensorUltrasonicValue = ultrasonic.read('c'); }
#endif

#ifdef MOISTURE_MODULE
void soilMoistureUpdate()
{
  sensorMoistureValues[0] = soilMoisture.read();
  sensorMoistureValues[1] = soilMoisture.readAndMap(0, 100);
}
#endif

#ifdef PIR_MODULE
void pirSensorUpdate()
{
  sensorPirValue = pirSensor.read();
  digitalWrite(LED_BUILTIN, sensorPirValue ? LOW : HIGH);
}
#endif

#ifdef MINI_FAN_MODULE
void miniFanUpdate() {}
#endif

#ifdef RELAY_MODULE
void relayUpdate() {}
#endif

#ifdef USB_SWITCH_MODULE
void usbSwitchUpdate() {}
#endif

#ifdef BUTTON_MODULE
void buttonUpdate() { button.tick(); }
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
  ts.addTask(t1Lcd);
  ts.addTask(t2Button);
  ts.addTask(t3Dht);
  ts.addTask(t4LightSensor);
  ts.addTask(t5Ultrasonic);
  ts.addTask(t6SoilMoisture);
  ts.addTask(t7PirSensor);
  ts.enableAll();
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
  button.setup(D2, INPUT_PULLUP, false);
  button.attachClick([]() { updateLcdScreenState(true); });
  button.attachDoubleClick([]() { updateLcdScreenState(false); });
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