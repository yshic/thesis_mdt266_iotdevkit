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

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method definitions ------------------------------------------- */

// Constructor
Ultrasonic::Ultrasonic(uint8_t _triggerPin, uint8_t _echoPin, unsigned long timeOut)
{
  triggerPin = _triggerPin;
  echoPin    = _echoPin;
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  timeout = timeOut;
}

unsigned int Ultrasonic::duration()
{
  unsigned int duration;
  // Reset trigger pin
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Start the ranging by setting the trigger pin high for 10 microsec
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);

  duration = pulseIn(echoPin, HIGH, timeout);

  return duration;
}

unsigned int Ultrasonic::read(char unit)
{
  switch (unit)
  {
    case 'c':
      return duration() / CM_DIVISOR;
      break;
    case 'm':
      return duration() / MM_DIVISOR;
      break;
    case 'i':
      return duration() / IN_DIVISOR;
      break;
    default:
      return -1;
  }
}

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */
