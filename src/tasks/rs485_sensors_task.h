/**
 * @file       rs485_sensors_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-12
 * @author     Tuan Nguyen
 *
 * @brief      Header file for RS485_SENSORS_TASK library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef RS485_SENSORS_TASK_H
  #define RS485_SENSORS_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  #include "bsp_rs485.h"

  /* Public defines ----------------------------------------------------- */
  #define RS485_SENSORS_TASK_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Task Declaration -------------------------------------------------- */
void soilRs485Task(void *pvParameters);

void soilRs485Setup();

void rs485Setup();

#endif // RS485_SENSORS_TASK_H

/* End of file -------------------------------------------------------- */