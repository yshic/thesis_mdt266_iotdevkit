#include "globals.h"

DHT20        dht20;
LCD_I2C      lcd(0x21, 16, 2);
LightSensor  lightSensor(A9);
Ultrasonic   ultrasonic(D1, D2);
PIRSensor    pirSensor(D7);
SoilMoisture soilMoisture(A0);
MiniFan      miniFan(A8);

// Relay         relay(D8);
// UsbSwitch     usbSwitch(D7, D6);
// ButtonHandler button(D2, false, true);

char ssid[32]     = "";
char password[32] = "";
