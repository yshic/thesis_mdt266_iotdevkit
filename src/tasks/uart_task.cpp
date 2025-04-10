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
void uartTask(void *pvParameters)
{
  for (;;)
  {
    if (uart1.available())
    {
      char incominByte = uart1.read();
      uart1.print("I received : ");
      uart1.println(incominByte);
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/* Private function prototypes ---------------------------------------- */

void uartSetup()
{
  uart1.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  xTaskCreate(&uartTask, "UART Task", 4096, nullptr, 1, nullptr);
}

/* End of file -------------------------------------------------------- */