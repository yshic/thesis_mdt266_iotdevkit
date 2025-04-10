/**
 * @file       uart_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-07
 * @author     Tuan Nguyen
 *
 * @brief      Header file for UART_TASK library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef UART_TASK_H
  #define UART_TASK_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define UART_TASK_LIB_VERSION (F("0.1.0"))

  #define RX_PIN                D7
  #define TX_PIN                D6
/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Task Declaration -------------------------------------------------- */

void uartTask(void *pvParameters);

void uartSetup();

#endif // UART_TASK_H

/* End of file -------------------------------------------------------- */