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
void bspGpioPinMode(uint8_t pin, uint8_t mode);

/* Digital Pin -------------------------------------------------------- */

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

/* Analog Pin -------------------------------------------------------- */

int  bspAnalogRead(uint8_t pin);
void bspAnalogReadResolution(uint8_t bits);
void bspAnalogWrite(uint8_t pin, int value);
void bspAnalogWriteResolution(uint8_t bits);

/* Advanced I/O ------------------------------------------------------- */

unsigned long bspGpioPulseIn(uint8_t pin, uint8_t state, unsigned long timeout);
unsigned long bspGpioPulseInLong(uint8_t pin, uint8_t state, unsigned long timeout);
uint8_t       bspGpioShiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
void          bspGpioShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
void          bspGpioTone(uint8_t pin, unsigned int frequency, unsigned long duration);
void          bspGpioNoTone(uint8_t pin);

#endif /* BSP_GPIO_H */

/* End of file -------------------------------------------------------- */
