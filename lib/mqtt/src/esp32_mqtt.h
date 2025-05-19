/**
 * @file       esp32_mqtt.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-05-17
 * @author     Tuan Nguyen
 *
 * @brief      Header file for ESP32_MQTT library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef ESP32_MQTT_H
  #define ESP32_MQTT_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "PubSubClient.h"
  /* Public defines ----------------------------------------------------- */
  #define ESP32_MQTT_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
class Esp32Mqtt
{
public:
private:
};

#endif // ESP32_MQTT_H

/* End of file -------------------------------------------------------- */