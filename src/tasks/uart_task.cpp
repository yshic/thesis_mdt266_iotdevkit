/**
 * @file       uart_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-07
 * @author     Tuan Nguyen
 *
 * @brief      Source file for uart_task.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "uart_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions   ------------------------------------------------- */
#ifdef UART_MODULE
void uartTask(void *pvParameters)
{
  for (;;)
  {
    if (uart1.available())
    {
      String message = uart1.readStringUntil('\n');
      uart1.println("Received: " + message);
      Serial.println("Received: " + message);
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

/* Private function prototypes ---------------------------------------- */

void uartSetup()
{
  uart1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  xTaskCreate(&uartTask, "UART Task", 4096, nullptr, 1, nullptr);
}
#endif // UART_MODULE
       /* End of file -------------------------------------------------------- */