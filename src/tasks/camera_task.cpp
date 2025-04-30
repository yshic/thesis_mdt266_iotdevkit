/**
 * @file       camera_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-04-27
 * @author     Tuan Nguyen
 *
 * @brief      Source file for camera_task.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "camera_task.h"
#include "globals.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */

/* Task definitions-------------------------------------------- */
void printResult(HUSKYLENSResult result)
{
  if (result.command == COMMAND_RETURN_BLOCK)
  {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter +
                   F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
    if (result.ID == 1)
    {
      lcd.setScreenState(LCD_SCREEN_CAMERA_FACE_DETECTED);
      doorServo.setDoorStatus(true);
      doorServo.writePos(180);
      vTaskDelay(pdMS_TO_TICKS(5000));
    }
    else
    {
      doorServo.setDoorStatus(false);
      doorServo.writePos(0);
    }
  }
  else if (result.command == COMMAND_RETURN_ARROW)
  {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin +
                   F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else
  {
    Serial.println("Object unknown!");
  }
}

void huskylensTask(void *pvParameters)
{
  for (;;)
  {
    if (huskylens.getCameraStatus())
    {
      if (!huskylens.request())
      {
#ifdef DEBUG_PRINT
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
#endif // DEBUG_PRINT
      }
      else if (!huskylens.isLearned())
      {
#ifdef DEBUG_PRINT
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
#endif // DEBUG_PRINT
      }
      else if (!huskylens.available())
      {
#ifdef DEBUG_PRINT
        Serial.println(F("No block or arrow appears on the screen!"));
#endif // DEBUG_PRINT
        lcd.setScreenState(LCD_SCREEN_CAMERA_NONE);
        doorServo.setDoorStatus(false);
        doorServo.writePos(0);
      }
      else
      {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
          huskylens.read();
          HUSKYLENSResult result = huskylens.getResult();
#ifdef DEBUG_PRINT
          printResult(result);
#endif // DEBUG_PRINT
        }
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void huskylensSetup()
{
  huskylens.begin(Wire);
  xTaskCreate(&huskylensTask, "Huskeylens Task", 4096, nullptr, 1, nullptr);
}
/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */