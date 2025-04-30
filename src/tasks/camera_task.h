/**
 * @file       camera_task.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-27
 * @author     Tuan Nguyen
 *
 * @brief      Header file for CAMERA_TASK library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef CAMERA_TASK_H
#define CAMERA_TASK_H

/* Includes ----------------------------------------------------------- */
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "HUSKYLENS.h"

/* Public defines ----------------------------------------------------- */
#define CAMERA_TASK_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Class Declaration -------------------------------------------------- */
void printResult(HUSKYLENSResult result);

void huskylensTask(void *pvParameters);

void huskylensSetup();

#endif // CAMERA_TASK_H

/* End of file -------------------------------------------------------- */