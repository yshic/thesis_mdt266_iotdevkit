/**
 * @file       usb_switch.h
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Header for USB Switch Module library
 *
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef USB_SWITCH_H
  #define USB_SWITCH_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */

/**
 * @brief  USB Switch control class for managing devices operations.
 *
 * This class provides methods to control a usb switch module, including setting the output value for each
 * output, the usb switch module has two output available: USB_OUTPUT1 and USB_OUTPUT2 toggling the output
 * state, checking the status of the output, and setting the output value as a percentage.
 *
 * @attention
 * - If using the grove connector, the yellow wire will control USB_OUTPUT1 and the white wire will control
 * USB_OUTPUT2.
 */
class UsbSwitch
{
public:
  /**
   * @brief  Initializes the USB Switch Module with specified pins.
   *
   * @param[in]  pin1  The pin first pin will control USB_OUTPUT1.
   * @param[in]  pin2  The second pin will ocntrol USB_OUTPUT2.
   *
   */
  UsbSwitch(uint8_t pin1, uint8_t pin2);

  /**
   * @brief  Sets the output value using a PWM value.
   *
   * This function sets the value of the output by writing a PWM value to the control pin.
   *
   * @param[in] outputNo  The number of the output (1 or 2, 0 for both)
   * @param[in] value     The output analog value (0-255).
   */
  void setOutputValue(uint8_t outputNo, int value);

  /**
   * @brief  Sets the output value as a percentage.
   *
   * This function sets the output value by converting a percentage value to a PWM value and writing it to
   * the specified control pin.
   *
   * @param[in]  outputNo    The output to be used (1 or 2, 0 for both)
   * @param[in]  percentage  The output value as a percentage (0-100%).
   *
   * @attention
   * - Testing with the pumping motor shows that the motor won't run with percentage value < 59
   *
   */
  void setOutputValuePercentage(uint8_t outputNo, int percentage);

  /**
   * @brief  Gets the current output value.
   *
   * This function returns the current output value as a PWM value.
   *
   * @param[in]  outputNo   The specified output to get the value from.
   *
   * @return  int  The current output value (0-255).
   */
  int getOutputValue(uint8_t outputNo);

  /**
   * @brief  Gets the current output value as a percentage.
   *
   * This function returns the current output value, converted from the PWM value to a percentage
   * (0-100%).
   *
   * @param[in]  outputNo   The specified output to get the value from.

   *
   * @return  int  The current output value as a percentage (0-100%).
   */
  int getOutputValuePercentage(uint8_t outputNo);

  /**
   * @brief  Toggles the output state between ON and OFF.
   *
   * This function toggles the state of the output. If the output is currently ON, it will be turned OFF, and
   * vice versa.
   *
   * @param[in]   outputNo    The output to be toggled (1 or 2, 0 for both)
   */
  void toggleOutput(uint8_t outputNo);

  /**
   * @brief  Checks if the status of the output is currently ON.
   *
   * @param[in]   outputNo   The output to get the status from.
   *
   * @return  true if the output is ON, false otherwise.
   */
  bool getStatus(uint8_t outputNo);

private:
  int  _pin1;     /** Pin of the first output (USB_OUTPUT1)*/
  int  _pin2;     /** Pin of the second output(USB_OUTPUT2)*/
  int  _value;    /** Value of the output */
  bool status[2]; /**< Status of the output (ON/OFF) */
};

#endif // USB_SWITCH_H

/* End of file -------------------------------------------------------- */