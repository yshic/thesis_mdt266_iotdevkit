/**
 * @file       relay.cpp
 * @license    This library is released under the MIT License
 * @version    0.1.0
 * @date       2024-11-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for Relay Module Library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "relay.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Class method Definitions ---------------------------------- */

// Constructor
Relay::Relay(int pin) : _pin(pin), status(false) { pinMode(_pin, OUTPUT); }

void Relay::on()
{
  status = true;
  digitalWrite(_pin, HIGH);
}

void Relay::off()
{
  status = false;
  digitalWrite(_pin, LOW);
}

void Relay::toggle()
{
  if (status)
  {
    off();
  }
  else
  {
    on();
  }
}

bool Relay::getStatus() { return status; }
/* Private function definitions --------------------------------------- */

/* End of file -------------------------------------------------------- */
