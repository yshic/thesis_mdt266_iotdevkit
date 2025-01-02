/**
 * @file       bsp_gpio.cpp
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_gpio
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_gpio.h"
/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
void bspGpioDigitalWrite(uint8_t pin, uint8_t state)
{
  switch (state)
  {
    case 0:
      digitalWrite(pin, state);
      break;
    case 1:
      digitalWrite(pin, state);
      break;
    default:
      break;
  }
}
bool bspGpioDigitalRead(uint8_t pin) { return digitalRead(pin) == 1 ? true : false; }

void bspPinMode(uint8_t pin, uint8_t mode)
{
  switch (mode)
  {
    case INPUT:
      pinMode(pin, INPUT);
      break;
    case INPUT_PULLUP:
      pinMode(pin, INPUT_PULLUP);
      break;
    case INPUT_PULLDOWN:
      pinMode(pin, INPUT_PULLDOWN);
      break;
    case OUTPUT:
      pinMode(pin, OUTPUT);
      break;
    case OPEN_DRAIN:
      pinMode(pin, OPEN_DRAIN);
      break;
    default:
      break;
  }
}
/* Private definitions ----------------------------------------------- */
/* End of file -------------------------------------------------------- */
