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
LightSensor lightSensor(LIGHT_SENSOR_PIN);
#endif

#ifdef ULTRASONIC_MODULE
Ultrasonic ultrasonic(ULTRASONIC_TRG_PIN, ULTRASONIC_ECH_PIN);
#endif

#ifdef PIR_MODULE
PIRSensor pirSensor(PIR_PIN);
#endif

#ifdef SOIL_MOISTURE_MODULE
SoilMoisture soilMoisture(SOIL_MOISTURE_PIN);
#endif

#ifdef ES_SOIL_RS485_MODULE
EsSoil7n1 esSoil;
#endif // ES_SOIL_RS485_MODULE

#ifdef HUSKYLENS_MODULE
HUSKYLENS huskylens;
#endif // HUSKYLENS_MODULE

#ifdef LCD_MODULE
LCD_I2C lcd(0x21, 16, 2);
#endif

#ifdef MINI_FAN_MODULE
MiniFan miniFan(MINI_FAN_PIN);
#endif

#ifdef SERVO_MODULE
Servo doorServo;
#endif

#ifdef RELAY_MODULE
Relay relay(RELAY_PIN);
#endif

#ifdef UNIT_4_RELAY_MODULE
Unit4Relay unit4Relay;
#endif

#ifdef USB_SWITCH_MODULE
UsbSwitch usbSwitch(USB_SWITCH_PIN_1, USB_SWITCH_PIN_2);
#endif

#ifdef BUTTON_MODULE
ButtonHandler button(BUTTON_PIN, false, true);
#endif

#ifndef ENTERPRISE_WIFI
char ssid[32]     = "";
char password[32] = "";
#endif // ENTERPRISE_WIFI

bool wifiConnected = false;
