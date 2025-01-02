/**
 * @file       bsp_gpio.h
 * @license    This project is released under the MIT License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     Tuan Nguyen
 *
 * @brief      Header file for bsp_gpio
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_GPIO_H
  #define BSP_GPIO_H

  /* Includes ----------------------------------------------------------- */
  #include <Arduino.h>

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief This function write the state of a digital pin
 *
 * @param[in] pin           GPIO Pin
 * @param[in] state         State to write to the GPIO Pin
 *
 * @attention None
 *
 * @return
 *
 * None
 */
void bspGpioDigitalWrite(uint8_t pin, uint8_t state);

/**
 * @brief This function read the state of a digital pin
 *
 * @param[in] pin      GPIO Pin
 *
 * @attention  If the pin isn’t connected to anything, bspGpioDigitalRead() can return either HIGH or LOW (and
 * this can change randomly).
 *
 * @return
 * - true - HIGH
 *
 * - false - LOW
 */
bool bspGpioDigitalRead(uint8_t pin);

/**
 * @brief This function set the mode of a GPIO pin
 *
 * @param[in] pin      GPIO Pin
 * @param[in] mode     GPIO Mode
 *
 * @attention  None
 *
 * @return
 *
 * None
 */
void bspPinMode(uint8_t pin, uint8_t mode);

#endif /* BSP_GPIO_H */

/* End of file -------------------------------------------------------- */
