/**
 * @file       ble_task.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-02-01
 * @author     Tuan Nguyen
 *
 * @brief      Source file for BLE Task
 *
 */

/* Includes ----------------------------------------------------------- */
#include "ble_task.h"
#include "globals.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */

/* Private variables -------------------------------------------------- */
BLECharacteristic *ssidCharacteristic;
BLECharacteristic *passwordCharacteristic;

/* Class method definitions ------------------------------------------- */

class SSIDCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic) override
  {
    std::string value = characteristic->getValue();
    if (value.length() < sizeof(bleSsid))
    {
      memset(bleSsid, 0, sizeof(bleSsid));
      memcpy(bleSsid, value.c_str(), value.length());
      // bleSsid[value.length()] = '\0';
    }
  }
};

class PasswordCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic) override
  {
    std::string value = characteristic->getValue();
    if (value.length() < sizeof(blePassword))
    {
      memset(blePassword, 0, sizeof(blePassword));
      memcpy(blePassword, value.c_str(), value.length());
    }
  }
};

/* Task definitions ------------------------------------------- */

void bleTask(void *pvParameters)
{
  BLEDevice::init("XIAO ESP32-S3");
  BLEServer  *pServer  = BLEDevice::createServer();
  BLEService *pService = pServer->createService("c3251bfb-5e94-4a01-93e7-6cb22b839cac");

  ssidCharacteristic =
  pService->createCharacteristic("b6c3dda3-c00b-4fa0-a34f-ca91de83a2e4", BLECharacteristic::PROPERTY_WRITE);
  passwordCharacteristic =
  pService->createCharacteristic("8f938de4-56db-45df-9333-912e290efed9", BLECharacteristic::PROPERTY_WRITE);

  // Attach callbacks
  ssidCharacteristic->setCallbacks(new SSIDCallback());
  passwordCharacteristic->setCallbacks(new PasswordCallback());
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID("c3251bfb-5e94-4a01-93e7-6cb22b839cac");
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  vTaskDelete(NULL);
}

void bleSetup() { xTaskCreate(bleTask, "BLETask", 4096, NULL, 1, NULL); }

/* End of file -------------------------------------------------------- */