/**
 * @file       relay_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-30
 * @author     Tuan Nguyen
 *
 * @brief      Header file for RELAY_TASK
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef RELAY_TASK_H
  #define RELAY_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define RELAY_TASK_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Task Declaration -------------------------------------------------- */
void unit4RelayTask(void *pvParameters);

void unit4RelaySetup();
#endif // RELAY_TASK_H

/* End of file -------------------------------------------------------- */