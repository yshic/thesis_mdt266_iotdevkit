/**
 * @file       relay_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-30
 * @author     Tuan Nguyen
 *
 * @brief      Source file for relay_task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "relay_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Tasks definitions-------------------------------------------- */
#ifdef UNIT_4_RELAY_MODULE
void unit4RelaySetup()
{
  unit4Relay.begin();
  unit4Relay.init(1);
  unit4Relay.relayAll(0);
  // xTaskCreate(unit4RelayTask, "Unit 4 Relay Task", 4096, NULL, 1 NULL);
}
#endif
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */