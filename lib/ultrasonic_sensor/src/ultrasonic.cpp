/**
 * @file       ultrasonic.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Ultrasonic Sensor Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "ultrasonic.h"
#include "bsp_gpio.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions ------------------------------------------- */

// Constructor
Ultrasonic::Ultrasonic(uint8_t _triggerPin, uint8_t _echoPin, unsigned long timeOut)
    : triggerPin(_triggerPin), echoPin(_echoPin), timeout(timeOut)
{
  bspGpioPinMode(triggerPin, OUTPUT);
  bspGpioPinMode(echoPin, INPUT);
}

unsigned int Ultrasonic::duration()
{
  unsigned int duration;
  // Reset trigger pin
  bspGpioDigitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Start the ranging by setting the trigger pin high for 10 microsec
  bspGpioDigitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  bspGpioDigitalWrite(triggerPin, LOW);

  duration = bspGpioPulseIn(echoPin, HIGH, timeout);

  return duration;
}

void Ultrasonic::read(char unit)
{
  switch (unit)
  {
    case 'c':
      distance[CM_INDEX] = duration() / 2 * CM_CONSTANT;
      break;
    case 'm':
      distance[MM_INDEX] = duration() / 2 * MM_CONSTANT;
      break;
    case 'i':
      distance[IN_INDEX] = duration() / 2 * IN_CONSTANT;
      break;
    default:
      break;
  }
}

unsigned int Ultrasonic::getDistance(char unit)
{
  switch (unit)
  {
    case 'c':
      return distance[CM_INDEX];
      break;
    case 'm':
      return distance[MM_INDEX];
      break;
    case 'i':
      return distance[IN_INDEX];
      break;
    default:
      return -1;
      break;
  }
}

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
