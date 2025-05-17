/* Includes ----------------------------------------------------------- */
#include "globals.h"

/* Public defines ----------------------------------------------------- */

/* Forward function declaration --------------------------------------- */

/* Public variables/object -------------------------------------------- */
SmartHome sh;
/* Public Class/Struct     -------------------------------------------- */

/* Function definitions ----------------------------------------------- */

/* Main code ---------------------------------------------------------- */
void setup()
{
#ifdef DEBUG_I2C
  Serial.begin(9600);
  Wire.begin();
  while (!Serial)
    ; // Wait for Serial to be ready
  scanI2CDevices();
#else
  bool devices[11]     = {1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1};
  bool connectivity[2] = {1, 0};

  sh.deviceEnable(devices);
  sh.connectivityEnable(connectivity);

  sh.serialSetup(false, true, true);
  sh.deviceSetup();
  sh.connectivitySetup();

  sh.iotServerConnect();
#endif // DEBUG_I2C
}

void loop()
{
#ifdef DEBUG_I2C
  scanI2CDevices();
#endif
}