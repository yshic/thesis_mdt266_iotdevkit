#include "globals.h"

#ifdef DHT20_MODULE
DHT20 dht20;
#endif

#ifdef SHT4X_MODULE
SHT4X sht40;
#endif

#ifdef BMP280_MODULE
BMP280 bmp280;
#endif

#ifdef AC_MEASURE_MODULE
AcMeasure acMeasure;
#endif

#ifdef LIGHT_SENSOR_MODULE
LightSensor lightSensor(A9);
#endif

#ifdef ULTRASONIC_MODULE
Ultrasonic ultrasonic(D1, D2);
#endif

#ifdef PIR_MODULE
PIRSensor pirSensor(D7);
#endif

#ifdef SOIL_MOISTURE_MODULE
SoilMoisture soilMoisture(A0);
#endif

#ifdef LCD_MODULE
LCD_I2C lcd(0x21, 16, 2);
#endif

#ifdef MINI_FAN_MODULE
MiniFan miniFan(A8);
#endif

#ifdef RELAY_MODULE
Relay relay(D8);
#endif

#ifdef UNIT_4_RELAY_MODULE
Unit4Relay unit4Relay;
#endif

#ifdef USB_SWITCH_MODULE
UsbSwitch usbSwitch(D7, D6);
#endif

#ifdef BUTTON_MODULE
ButtonHandler button(D2, false, true);
#endif

#ifndef ENTERPRISE_WIFI
char ssid[32]     = "";
char password[32] = "";
#endif // ENTERPRISE_WIFI

bool wifiConnected = false;
