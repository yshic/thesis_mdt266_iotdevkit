/**
 * @file       HUSKYLENS.h
 * @license    This project is released under GNU Lesser General Public License.
 * @version    1.0.0
 * @date       2024-12-31
 * @author     [Angelo](Angelo.qiao@dfrobot.com)
 * @author     [Robert](robert@dfrobot.com)
 * @author     Tuan Nguyen
 *
 * @brief      Header file for HUSKYLENS
 *
 * @note       None
 * @example    None
 */
#include "Arduino.h"
#include "HuskyLensProtocolCore.h"
#include "Wire.h"

#ifndef _HUSKYLENS_H
  #define _HUSKYLENS_H

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

enum protocolCommand
{
  COMMAND_REQUEST = 0x20,
  COMMAND_REQUEST_BLOCKS,
  COMMAND_REQUEST_ARROWS,
  COMMAND_REQUEST_LEARNED,
  COMMAND_REQUEST_BLOCKS_LEARNED,
  COMMAND_REQUEST_ARROWS_LEARNED,
  COMMAND_REQUEST_BY_ID,
  COMMAND_REQUEST_BLOCKS_BY_ID,
  COMMAND_REQUEST_ARROWS_BY_ID,

  COMMAND_RETURN_INFO,
  COMMAND_RETURN_BLOCK,
  COMMAND_RETURN_ARROW,

  COMMAND_REQUEST_KNOCK,
  COMMAND_REQUEST_ALGORITHM,

  COMMAND_RETURN_OK,
  COMMAND_REQUEST_CUSTOMNAMES,
  COMMAND_REQUEST_PHOTO,
  COMMAND_REQUEST_SEND_PHOTO,
  COMMAND_REQUEST_SEND_KNOWLEDGES,
  COMMAND_REQUEST_RECEIVE_KNOWLEDGES,
  COMMAND_REQUEST_CUSTOM_TEXT,
  COMMAND_REQUEST_CLEAR_TEXT,

  COMMAND_REQUEST_LEARN,
  COMMAND_REQUEST_FORGET,

  COMMAND_REQUEST_SEND_SCREENSHOT,
  COMMAND_REQUEST_SAVE_SCREENSHOT,
  COMMAND_REQUEST_LOAD_AI_FRAME_FROM_USB,
  COMMAND_REQUEST_IS_PRO,
  COMMAND_REQUEST_FIRMWARE_VERSION = 0x3C,

  COMMAND_REQUEST_SENSOR,
};

typedef struct
{
  uint8_t nameDataSize;
  uint8_t id;
  // uint8_t numOfNames;
  // uint8_t algorithim;
  char dataBuffer[21];
} ProtocolCustomNameHeader_t;

typedef struct
{
  uint8_t *dataBuffer;
} ProtocolReceivedKnowledges_t;

typedef struct
{
  uint16_t x;
  uint8_t  y;
  uint8_t  textSize;
  uint8_t  text[21];
} ProtocolCustomText_t;

typedef struct
{
  uint8_t  length;
  uint8_t *data;
} ProtocolFirmwareVersion_t;

typedef struct
{
  uint8_t command;
  union
  {
    int16_t first;
    int16_t xCenter;
    int16_t xOrigin;
    int16_t protocolSize;
    int16_t algorithmType;
    int16_t requestID;
  };
  union
  {
    int16_t second;
    int16_t yCenter;
    int16_t yOrigin;
    int16_t knowledgeSize;
  };
  union
  {
    int16_t third;
    int16_t width;
    int16_t xTarget;
    int16_t frameNum;
  };
  union
  {
    int16_t fourth;
    int16_t height;
    int16_t yTarget;
  };
  union
  {
    int16_t fifth;
    int16_t ID;
  };

  ProtocolCustomNameHeader_t   customNameHeader;
  ProtocolReceivedKnowledges_t receivedKnowledges;
  ProtocolCustomText_t         customText;
  ProtocolFirmwareVersion_t    firmwareVersion;
} Protocol_t;

enum protocolAlgorithm
{
  ALGORITHM_FACE_RECOGNITION,
  ALGORITHM_OBJECT_TRACKING,
  ALGORITHM_OBJECT_RECOGNITION,
  ALGORITHM_LINE_TRACKING,
  ALGORITHM_COLOR_RECOGNITION,
  ALGORITHM_TAG_RECOGNITION,
  ALGORITHM_OBJECT_CLASSIFICATION,
};

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

typedef Protocol_t HUSKYLENSResult;

/**
 * @brief Manages communication with the HuskyLens vision sensor.
 *
 * The `HUSKYLENS` class provides an interface for interacting with the HuskyLens vision sensor over I2C or
 * serial communication. It supports multiple vision algorithms (e.g., face recognition, object tracking),
 * retrieving blocks and arrows with associated IDs, managing learned objects, and controlling UI elements
 * like custom text and screenshots. The class handles protocol-level communication, data parsing, and result
 * storage for efficient access.
 *
 * ### Features:
 *
 * - Supports multiple algorithms: face recognition, object tracking, line tracking, and more.
 *
 * - Retrieves blocks (rectangular regions) and arrows (directional indicators) with position and ID data.
 *
 * - Manages learned objects with custom names and IDs for identification.
 *
 * - Provides SD card operations for saving/loading models, photos, and screenshots.
 *
 * - Allows custom text display on the HuskyLens UI with position control.
 *
 * - Checks firmware version and Pro model status.
 *
 * ### Usage:
 *
 * Instantiate the class and call `begin()` with a `TwoWire` (I2C) or `Stream` (serial) object to initialize
 * communication. Select an algorithm with `writeAlgorithm()`. Use `request()` methods to fetch blocks or
 * arrows (all, by ID, or learned), then access results with `read()`, `get()`, or specific
 * `getBlock()`/`getArrow()` methods. Manage learned objects with `writeLearn()` or `writeForget()`, and use
 * SD card or UI functions as needed.
 *
 * ### Dependencies:
 *
 * - Requires an Arduino-compatible board with I2C or serial support.
 * - Depends on `Wire.h` for I2C communication and `Arduino.h` for core functionality.
 * - Requires `HuskyLensProtocolCore.h` for protocol handling.
 * - HuskyLens must be connected to the appropriate interface (I2C address 0x32 or serial port).
 */
class HUSKYLENS
{
private:
  TwoWire        *wire;
  Stream         *stream;
  unsigned long   timeOutDuration = 100;
  unsigned long   timeOutTimer;
  int16_t         currentIndex = 0;
  Protocol_t      protocolCache;
  HUSKYLENSResult result;
  // custom
  bool status = false;

  void protocolWrite(uint8_t *buffer, int length)
  {
    if (wire)
    {
      wire->beginTransmission(0x32);
      wire->write(buffer, length);
      wire->endTransmission();
    }
    else if (stream)
    {
      stream->write(buffer, length);
    }
  }

  void timerBegin() { timeOutTimer = millis(); }

  bool timerAvailable() { return (millis() - timeOutTimer > timeOutDuration); }

  bool protocolAvailable()
  {
    if (wire)
    {
      if (!wire->available())
      {
        wire->requestFrom(0x32, 16);
      }
      while (wire->available())
      {
        int result = wire->read();
        if (husky_lens_protocol_receive(result))
        {
          return true;
        }
      }
    }
    else if (stream)
    {
      while (stream->available())
      {
        int result = stream->read();
        if (husky_lens_protocol_receive(result))
        {
          return true;
        }
      }
    }

    return false;
  }

  Protocol_t  protocolInfo;
  Protocol_t *protocolPtr = NULL;

  bool processReturn()
  {
    currentIndex = 0;
    if (!wait(COMMAND_RETURN_INFO))
      return false;
    protocolReadReturnInfo(protocolInfo);
    protocolPtr =
    (Protocol_t *) realloc(protocolPtr, max((int) protocolInfo.protocolSize, 1) * sizeof(Protocol_t));

    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (!wait())
        return false;
      if (protocolReadReturnBlock(protocolPtr[i]))
        continue;
      else if (protocolReadReturnArrow(protocolPtr[i]))
        continue;
      else
        return false;
    }
    return true;
  }

  HUSKYLENSResult resultDefault;

  bool wait(uint8_t command = 0)
  {
    timerBegin();
    while (!timerAvailable())
    {
      if (protocolAvailable())
      {
        if (command)
        {
          if (husky_lens_protocol_read_begin(command))
            return true;
        }
        else
        {
          return true;
        }
      }
    }
    return false;
  }

  bool readKnock()
  {
    for (int i = 0; i < 5; i++)
    {
      protocolWriteRequestKnock();
      if (wait(COMMAND_RETURN_OK))
      {
        return true;
      }
    }
    return false;
  }

public:
  /**
   * @brief Constructs a HUSKYLENS object.
   *
   * Initializes the object with default settings, preparing it for communication setup.
   *
   * @param[in] None
   */
  HUSKYLENS(/* args */)
  {
    wire                  = NULL;
    stream                = NULL;
    resultDefault.command = -1;
    resultDefault.first   = -1;
    resultDefault.second  = -1;
    resultDefault.third   = -1;
    resultDefault.fourth  = -1;
    resultDefault.fifth   = -1;
  }

  /**
   * @brief Destructor for the HUSKYLENS object.
   *
   * Cleans up allocated resources, if any.
   *
   * @param[in] None
   */
  ~HUSKYLENS() {}

  /**
   * @brief Initializes communication with the HuskyLens over serial.
   *
   * Attempts to establish a connection with the HuskyLens via a serial interface and verifies communication.
   *
   * @param[in] streamInput The `Stream` object (e.g., Serial, SoftwareSerial) for serial communication.
   *
   * @attention Ensure the HuskyLens is connected to the serial port before calling.
   *
   * @return bool `true` if connection and communication are successful, `false` otherwise.
   */
  bool begin(Stream &streamInput)
  {
    stream = &streamInput;
    wire   = NULL;
    return readKnock();
  }

  /**
   * @brief Initializes communication with the HuskyLens over I2C.
   *
   * Attempts to establish a connection with the HuskyLens at I2C address 0x32 and verifies communication.
   *
   * @param[in] streamInput The `TwoWire` object for I2C communication.
   *
   * @attention Ensure the HuskyLens is connected to the I2C bus before calling.
   *
   * @return bool `true` if connection and communication are successful, `false` otherwise.
   */
  bool begin(TwoWire &streamInput)
  {
    stream = NULL;
    wire   = &streamInput;
    return readKnock();
  }

  /**
   * @brief Sets the timeout duration for communication.
   *
   * Configures the maximum time to wait for a response from the HuskyLens (default: 100ms).
   *
   * @param[in] timeOutDurationInput Timeout duration in milliseconds.
   *
   * @attention Set an appropriate timeout to balance responsiveness and reliability.
   */
  void setTimeOutDuration(unsigned long timeOutDurationInput) { timeOutDuration = timeOutDurationInput; }

  /**
   * @brief Requests all blocks and arrows from the HuskyLens.
   *
   * Initiates a request for all detected blocks and arrows, storing results for subsequent access.
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool request()
  {
    protocolWriteRequest();
    return processReturn();
  }

  /**
   * @brief Requests blocks and arrows with a specific ID.
   *
   * Requests only blocks and arrows tagged with the specified ID.
   *
   * @param[in] ID The target ID of blocks and arrows (0 for unlearned, 1+ for learned).
   *
   * @attention Ensure the ID is valid and the HuskyLens is initialized.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool request(int16_t ID)
  {
    Protocol_t protocol;
    protocol.requestID = ID;
    protocolWriteRequestByID(protocol);
    return processReturn();
  }

  /**
   * @brief Requests all blocks from the HuskyLens.
   *
   * Initiates a request for all detected blocks, regardless of ID.
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestBlocks()
  {
    protocolWriteRequestBlocks();
    return processReturn();
  }

  /**
   * @brief Requests blocks with a specific ID.
   *
   * Requests only blocks tagged with the specified ID.
   *
   * @param[in] ID The target ID of blocks (0 for unlearned, 1+ for learned).
   *
   * @attention Ensure the ID is valid and the HuskyLens is initialized.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestBlocks(int16_t ID)
  {
    Protocol_t protocol;
    protocol.requestID = ID;
    protocolWriteRequestBlocksByID(protocol);
    return processReturn();
  }

  /**
   * @brief Requests all arrows from the HuskyLens.
   *
   * Initiates a request for all detected arrows, regardless of ID.
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestArrows()
  {
    protocolWriteRequestArrows();
    return processReturn();
  }

  /**
   * @brief Requests arrows with a specific ID.
   *
   * Requests only arrows tagged with the specified ID.
   *
   * @param[in] ID The target ID of arrows (0 for unlearned, 1+ for learned).
   *
   * @attention Ensure the ID is valid and the HuskyLens is initialized.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestArrows(int16_t ID)
  {
    Protocol_t protocol;
    protocol.requestID = ID;
    protocolWriteRequestArrowsByID(protocol);
    return processReturn();
  }

  /**
   * @brief Requests all learned blocks and arrows.
   *
   * Requests blocks and arrows with IDs >= 1 (learned objects).
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestLearned()
  {
    protocolWriteRequestLearned();
    return processReturn();
  }

  /**
   * @brief Requests all learned blocks.
   *
   * Requests blocks with IDs >= 1 (learned objects).
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestBlocksLearned()
  {
    protocolWriteRequestBlocksLearned();
    return processReturn();
  }

  /**
   * @brief Requests all learned arrows.
   *
   * Requests arrows with IDs >= 1 (learned objects).
   *
   * @param[in] None
   *
   * @attention Ensure the HuskyLens is initialized with `begin()`.
   *
   * @return bool `true` if the request is successful, `false` if communication fails.
   */
  bool requestArrowsLearned()
  {
    protocolWriteRequestArrowsLearned();
    return processReturn();
  }

  /**
   * @brief Returns the number of blocks and arrows available to read.
   *
   * Provides the count of remaining blocks and arrows in the buffer after a request.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int The number of blocks and arrows left to read.
   */
  int available()
  {
    int result   = count();
    currentIndex = min((int) currentIndex, result);
    return result - currentIndex;
  }

  /**
   * @brief Reads the next block or arrow from the buffer.
   *
   * Retrieves the next available block or arrow, advancing the internal index.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call and available data (`available()` > 0).
   *
   * @return HUSKYLENSResult The block or arrow data, including position and ID.
   */
  void read() { result = (get(currentIndex++)); }

  // CUSTOM METHODS

  /**
   * @brief Retrieves the current result.
   *
   * Returns the most recently read block or arrow result.
   *
   * @param[in] None
   *
   * @attention Requires a prior call to `read()`.
   *
   * @return HUSKYLENSResult The current block or arrow data.
   */
  HUSKYLENSResult getResult() { return result; }

  /**
   * @brief Sets the camera status.
   *
   * Configures a custom status flag for the camera (not directly tied to hardware).
   *
   * @param[in] state The desired status (`true` for ON, `false` for OFF).
   */
  void setCameraStatus(bool state) { status = state; }

  /**
   * @brief Retrieves the camera status.
   *
   * Returns the custom status flag set for the camera.
   *
   * @param[in] None
   *
   * @return bool The current camera status (`true` for ON, `false` for OFF).
   */
  bool getCameraStatus() { return status; }

  /**
   * @brief Checks if any objects have been learned.
   *
   * Determines if the HuskyLens has learned any objects (IDs >= 1).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return bool `true` if at least one object is learned, `false` otherwise.
   */
  bool isLearned() { return countLearnedIDs(); }

  /**
   * @brief Checks if a specific ID has been learned.
   *
   * Verifies if an object with the specified ID has been learned.
   *
   * @param[in] ID The target ID to check (1+ for learned objects).
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return bool `true` if the ID is learned, `false` otherwise.
   */
  bool isLearned(int ID) { return (ID <= countLearnedIDs()); }

  /**
   * @brief Retrieves the number of frames processed.
   *
   * Returns the total number of frames processed by the HuskyLens.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The frame count.
   */
  int16_t frameNumber() { return protocolInfo.frameNum; }

  /**
   * @brief Retrieves the number of learned IDs.
   *
   * Returns the count of unique learned objects (e.g., faces, colors, lines).
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The number of learned IDs.
   */
  int16_t countLearnedIDs() { return protocolInfo.knowledgeSize; }

  /**
   * @brief Retrieves the total count of blocks and arrows.
   *
   * Returns the number of all detected blocks and arrows in the current request.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The total count of blocks and arrows.
   */
  int16_t count() { return protocolInfo.protocolSize; }

  /**
   * @brief Retrieves the count of blocks and arrows with a specific ID.
   *
   * Returns the number of blocks and arrows tagged with the specified ID.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of blocks and arrows with the specified ID.
   */
  int16_t count(int16_t ID)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].ID == ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the total count of blocks.
   *
   * Returns the number of all detected blocks in the current request.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The total count of blocks.
   */
  int16_t countBlocks()
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the count of blocks with a specific ID.
   *
   * Returns the number of blocks tagged with the specified ID.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of blocks with the specified ID.
   */
  int16_t countBlocks(int16_t ID)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID == ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the total count of arrows.
   *
   * Returns the number of all detected arrows in the current request.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The total count of arrows.
   */
  int16_t countArrows()
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the count of arrows with a specific ID.
   *
   * Returns the number of arrows tagged with the specified ID.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of arrows with the specified ID.
   */
  int16_t countArrows(int16_t ID)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID == ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the count of all learned blocks and arrows.
   *
   * Returns the number of blocks and arrows with IDs >= 1.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of learned blocks and arrows.
   */
  int16_t countLearned()
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the count of learned blocks.
   *
   * Returns the number of blocks with IDs >= 1.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of learned blocks.
   */
  int16_t countBlocksLearned()
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves the count of learned arrows.
   *
   * Returns the number of arrows with IDs >= 1.
   *
   * @param[in] None
   *
   * @attention Requires a prior successful `request()` call.
   *
   * @return int16_t The count of learned arrows.
   */
  int16_t countArrowsLearned()
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID)
        counter++;
    }
    return counter;
  }

  /**
   * @brief Retrieves a block or arrow by index.
   *
   * Returns the block or arrow at the specified index from the current request.
   *
   * @param[in] index The index of the block or arrow (0 to `count()` - 1).
   *
   * @attention Ensure the index is valid (`index < count()`).
   *
   * @return HUSKYLENSResult The block or arrow data, or a default result if the index is invalid.
   */
  HUSKYLENSResult get(int16_t index)
  {
    if (index < count())
    {
      return protocolPtr[index];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a block or arrow with a specific ID by index.
   *
   * Returns the block or arrow with the specified ID at the given index.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   * @param[in] index The index of the block or arrow for the ID (0 to `count(ID)` - 1).
   *
   * @attention Ensure the index is valid (`index < count(ID)`).
   *
   * @return HUSKYLENSResult The block or arrow data, or a default result if invalid.
   */
  HUSKYLENSResult get(int16_t ID, int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].ID == ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a block by index.
   *
   * Returns the block at the specified index from the current request.
   *
   * @param[in] index The index of the block (0 to `countBlocks()` - 1).
   *
   * @attention Ensure the index is valid (`index < countBlocks()`).
   *
   * @return HUSKYLENSResult The block data, or a default result if the index is invalid.
   */
  HUSKYLENSResult getBlock(int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a block with a specific ID by index.
   *
   * Returns the block with the specified ID at the given index.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   * @param[in] index The index of the block for the ID (0 to `countBlocks(ID)` - 1).
   *
   * @attention Ensure the index is valid (`index < countBlocks(ID)`).
   *
   * @return HUSKYLENSResult The block data, or a default result if invalid.
   */
  HUSKYLENSResult getBlock(int16_t ID, int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID == ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves an arrow by index.
   *
   * Returns the arrow at the specified index from the current request.
   *
   * @param[in] index The index of the arrow (0 to `countArrows()` - 1).
   *
   * @attention Ensure the index is valid (`index < countArrows()`).
   *
   * @return HUSKYLENSResult The arrow data, or a default result if the index is invalid.
   */
  HUSKYLENSResult getArrow(int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves an arrow with a specific ID by index.
   *
   * Returns the arrow with the specified ID at the given index.
   *
   * @param[in] ID The target ID (0 for unlearned, 1+ for learned).
   * @param[in] index The index of the arrow for the ID (0 to `countArrows(ID)` - 1).
   *
   * @attention Ensure the index is valid (`index < countArrows(ID)`).
   *
   * @return HUSKYLENSResult The arrow data, or a default result if invalid.
   */
  HUSKYLENSResult getArrow(int16_t ID, int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID == ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a learned block or arrow by index.
   *
   * Returns the learned block or arrow (ID >= 1) at the specified index.
   *
   * @param[in] index The index of the learned block or arrow (0 to `countLearned()` - 1).
   *
   * @attention Ensure the index is valid (`index < countLearned()`).
   *
   * @return HUSKYLENSResult The block or arrow data, or a default result if invalid.
   */
  HUSKYLENSResult getLearned(int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a learned block by index.
   *
   * Returns the learned block (ID >= 1) at the specified index.
   *
   * @param[in] index The index of the learned block (0 to `countBlocksLearned()` - 1).
   *
   * @attention Ensure the index is valid (`index < countBlocksLearned()`).
   *
   * @return HUSKYLENSResult The block data, or a default result if invalid.
   */
  HUSKYLENSResult getBlockLearned(int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_BLOCK && protocolPtr[i].ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Retrieves a learned arrow by index.
   *
   * Returns the learned arrow (ID >= 1) at the specified index.
   *
   * @param[in] index The index of the learned arrow (0 to `countArrowsLearned()` - 1).
   *
   * @attention Ensure the index is valid (`index < countArrowsLearned()`).
   *
   * @return HUSKYLENSResult The arrow data, or a default result if invalid.
   */
  HUSKYLENSResult getArrowLearned(int16_t index)
  {
    int16_t counter = 0;
    for (int i = 0; i < protocolInfo.protocolSize; i++)
    {
      if (protocolPtr[i].command == COMMAND_RETURN_ARROW && protocolPtr[i].ID)
        if (index == counter++)
          return protocolPtr[i];
    }
    return resultDefault;
  }

  /**
   * @brief Switches the HuskyLens to the specified algorithm.
   *
   * Configures the HuskyLens to use the target vision algorithm (e.g., face recognition, line tracking).
   *
   * @param[in] algorithmType The target algorithm (see `protocolAlgorithm` enum).
   *
   * @attention Ensure the algorithm is supported by the HuskyLens model.
   *
   * @return bool `true` if the switch is successful, `false` if communication fails.
   */
  bool writeAlgorithm(protocolAlgorithm algorithmType)
  {
    Protocol_t protocol;
    protocol.algorithmType = algorithmType;
    protocolWriteRequestAlgorithm(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Instructs the HuskyLens to learn an object with a specific ID.
   *
   * Triggers learning for the current object, assigning it the specified ID (used in Object Classification).
   *
   * @param[in] ID The ID to assign to the learned object (1+).
   *
   * @attention Only works in Object Classification mode.
   *
   * @return bool `true` if learning is successful, `false` if communication fails.
   */
  bool writeLearn(int ID)
  {
    Protocol_t protocol;
    protocol.requestID = ID;
    protocolWriteRequestLearn(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Instructs the HuskyLens to forget all learned objects.
   *
   * Clears all learned objects (used in Object Classification).
   *
   * @param[in] None
   *
   * @attention Only works in Object Classification mode.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool writeForget()
  {
    protocolWriteRequestForget();
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Sets sensor parameters.
   *
   * Configures the HuskyLens with three sensor parameters (purpose depends on context).
   *
   * @param[in] sensor0 First sensor parameter.
   * @param[in] sensor1 Second sensor parameter.
   * @param[in] sensor2 Third sensor parameter.
   *
   * @attention Ensure parameters are valid for the current algorithm.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool writeSensor(int sensor0, int sensor1, int sensor2)
  {
    Protocol_t protocol;
    protocol.first  = sensor0;
    protocol.second = sensor1;
    protocol.third  = sensor2;
    protocolWriteRequestSensor(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Sets a custom name for a learned object.
   *
   * Assigns a custom name (up to 20 characters) to a learned object with the specified ID.
   *
   * @param[in] name The custom name for the object.
   * @param[in] id The ID of the learned object (1+).
   *
   * @attention The name must be 20 characters or fewer.
   *
   * @return bool `true` if the operation is successful, `false` if the name is too long or communication
   * fails.
   */
  bool setCustomName(String name, uint8_t id)
  {
    const char *nameC = name.c_str();
    Protocol_t  protocol;
    protocol.customNameHeader.nameDataSize = strlen(nameC);
    protocol.customNameHeader.id           = id;
    if (protocol.customNameHeader.nameDataSize > 20)
    {
      return false;
    }
    memcpy(protocol.customNameHeader.dataBuffer, nameC, protocol.customNameHeader.nameDataSize);
    protocolWriteRequestCustomNames(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Saves a photo to the SD card.
   *
   * Captures and saves a photo from the HuskyLens camera to the SD card.
   *
   * @param[in] None
   *
   * @attention Requires an SD card inserted in the HuskyLens. A UI popup may appear on error.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool savePictureToSDCard()
  {
    Protocol_t protocol;
    protocolWriteRequestPhoto(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Saves the current algorithm model to the SD card.
   *
   * Saves the learned object data for the current algorithm to a file named
   * "AlgorithmName_Backup_FileNum.conf".
   *
   * @param[in] fileNum The file number to include in the filename.
   *
   * @attention Requires an SD card inserted. A UI popup may appear on error.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool saveModelToSDCard(int fileNum)
  {
    Protocol_t protocol;
    protocol.first = fileNum;
    protocolWriteRequestSendKnowledges(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Loads an algorithm model from the SD card.
   *
   * Loads a model file ("AlgorithmName_Backup_FileNum.conf") from the SD card and refreshes the current
   * algorithm.
   *
   * @param[in] fileNum The file number of the model to load.
   *
   * @attention Requires an SD card inserted with the specified file. A UI popup may appear on error.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool loadModelFromSDCard(int fileNum)
  {
    Protocol_t protocol;
    protocol.first = fileNum;
    protocolWriteRequestReceiveKnowledges(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Clears all custom text from the HuskyLens UI.
   *
   * Removes all user-defined text displayed on the HuskyLens screen.
   *
   * @param[in] None
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool clearCustomText()
  {
    Protocol_t protocol;
    protocolWriteRequestClearText(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Displays custom text on the HuskyLens UI.
   *
   * Places a text string (up to 20 characters) at the specified (x, y) coordinates on the HuskyLens screen.
   * Up to 10 texts can be displayed, with new texts overwriting older ones in a circular buffer. Texts at the
   * same coordinates replace existing text.
   *
   * @param[in] text The text to display (max 20 characters).
   * @param[in] x The x-coordinate of the top-left corner (0–320).
   * @param[in] y The y-coordinate of the top-left corner (0–240).
   *
   * @attention Ensure the text is 20 characters or fewer and coordinates are within bounds.
   *
   * @return bool `true` if the operation is successful, `false` if the text is too long or communication
   * fails.
   */
  bool customText(String text, uint16_t x, uint8_t y)
  {
    const char *textC = text.c_str();
    Protocol_t  protocol;
    protocol.customText.textSize = strlen(textC);
    if (protocol.customText.textSize > 20)
    {
      return false;
    }
    protocol.customText.x = x;
    protocol.customText.y = y;
    memcpy(protocol.customText.text, textC, protocol.customText.textSize);
    protocolWriteRequestCustomText(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Saves a screenshot of the HuskyLens UI to the SD card.
   *
   * Captures and saves the current HuskyLens UI as an image to the SD card.
   *
   * @param[in] None
   *
   * @attention Requires an SD card inserted. A UI popup may appear on error.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool saveScreenshotToSDCard()
  {
    Protocol_t protocol;
    protocolWriteRequestSaveScreenshot(protocol);
    return wait(COMMAND_RETURN_OK);
  }

  /**
   * @brief Checks if the HuskyLens is a Pro model.
   *
   * Queries the HuskyLens to determine if it is a Pro or Standard model.
   *
   * @param[in] None
   *
   * @return bool `true` if the HuskyLens is a Pro model, `false` if Standard or if communication fails.
   */
  bool isPro()
  {
    Protocol_t protocolRequest;
    Protocol_t protocolResonse;
    protocolWriteRequestIsPro(protocolRequest);
    wait(COMMAND_RETURN_INFO);
    if (protocolReadRequestIsPro(protocolResonse))
    {
      return protocolResonse.first;
    }
    else
    {
      return false;
    }
  }

  #define HUSKYLENS_FIRMWARE_VERSION "0.4.1"

  /**
   * @brief Checks if the firmware version is up to date.
   *
   * Verifies the HuskyLens firmware version and displays a UI message if it is outdated.
   *
   * @param[in] None
   *
   * @return bool `true` if the check is performed successfully, `false` if communication fails.
   */
  bool checkFirmwareVersion()
  {
    writeFirmwareVersion(HUSKYLENS_FIRMWARE_VERSION);
    return true;
  }

  /**
   * @brief Writes a firmware version to the HuskyLens.
   *
   * Sends a firmware version string to the HuskyLens for verification or update purposes.
   *
   * @param[in] version The firmware version string to send.
   *
   * @attention Ensure the version string is valid and compatible with the HuskyLens.
   *
   * @return bool `true` if the operation is successful, `false` if communication fails.
   */
  bool writeFirmwareVersion(String version)
  {
    Protocol_t protocol;
    uint8_t    length           = version.length();
    uint8_t    data[length + 2] = {length};
    version.toCharArray((char *) data + 1, length + 1);
    protocol.firmwareVersion.length = length + 1;
    protocol.firmwareVersion.data   = data;
    protocolWriteRequestFirmwareVersion(protocol);
    return wait(COMMAND_RETURN_OK);
  }
  ////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////

  void protocolWriteCommand(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    int      length  = husky_lens_protocol_write_end();
    protocolWrite(buffer, length);
  }

  bool protocolReadCommand(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command = command;
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  void protocolWriteFiveInt16(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    husky_lens_protocol_write_int16(protocol.first);
    husky_lens_protocol_write_int16(protocol.second);
    husky_lens_protocol_write_int16(protocol.third);
    husky_lens_protocol_write_int16(protocol.fourth);
    husky_lens_protocol_write_int16(protocol.fifth);
    int length = husky_lens_protocol_write_end();
    protocolWrite(buffer, length);
  }

  bool protocolReadFiveInt16(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command = command;
      protocol.first   = husky_lens_protocol_read_int16();
      protocol.second  = husky_lens_protocol_read_int16();
      protocol.third   = husky_lens_protocol_read_int16();
      protocol.fourth  = husky_lens_protocol_read_int16();
      protocol.fifth   = husky_lens_protocol_read_int16();
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  void protocolWriteOneInt16(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    husky_lens_protocol_write_int16(protocol.first);
    int length = husky_lens_protocol_write_end();
    protocolWrite(buffer, length);
  }

  bool protocolReadOneInt16(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command = command;
      protocol.first   = husky_lens_protocol_read_int16();
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  bool protocolReadCustomNameHeader(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command                       = command;
      protocol.customNameHeader.id           = husky_lens_protocol_read_uint8();
      protocol.customNameHeader.nameDataSize = husky_lens_protocol_read_uint8();
      for (int i = 0; i < protocol.customNameHeader.nameDataSize; i++)
      {
        if (i > 20)
        {
          break;
        }
        protocol.customNameHeader.dataBuffer[i] = husky_lens_protocol_read_uint8();
      }
      protocol.customNameHeader.dataBuffer[20] = 0x00;
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  void protocolWriteCustomNameHeader(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    husky_lens_protocol_write_uint8(protocol.customNameHeader.id);
    husky_lens_protocol_write_uint8(protocol.customNameHeader.nameDataSize);
    for (int i = 0; i < 20; i++)
    {
      husky_lens_protocol_write_uint8(protocol.customNameHeader.dataBuffer[i]);
    }
    husky_lens_protocol_write_uint8(0x0);
    int length = husky_lens_protocol_write_end();
    protocolWrite(buffer, length);
  }

  bool protocolReadReceivedKnowledges(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool protocolWriteReceivedKnowledges(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

  bool protocolReadCustomTextRecv(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command             = command;
      protocol.customText.textSize = husky_lens_protocol_read_uint8();
      protocol.customText.x        = husky_lens_protocol_read_uint8() + husky_lens_protocol_read_uint8();
      protocol.customText.y        = husky_lens_protocol_read_uint8();
      char buf[protocol.customText.textSize];
      for (int i = 0; i < protocol.customText.textSize; i++)
      {
        if (i > 20)
        {
          break;
        }
        protocol.customText.text[i] = husky_lens_protocol_read_uint8();
      }
      protocol.customText.text[20] = 0x00;
      // Null terminate the string
      protocol.customText.text[protocol.customText.textSize] = 0x00;
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  bool protocolWriteCustomTextRecv(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    husky_lens_protocol_write_uint8(protocol.customText.textSize);
    if (protocol.customText.x >= 255)
    {
      husky_lens_protocol_write_uint8(0xFF);
    }
    else
    {
      husky_lens_protocol_write_uint8(0x00);
    }
    husky_lens_protocol_write_uint8(protocol.customText.x & 0xFF);
    husky_lens_protocol_write_uint8(protocol.customText.y);
    for (int i = 0; i < protocol.customText.textSize; i++)
    {
      husky_lens_protocol_write_uint8(protocol.customText.text[i]);
    }
    husky_lens_protocol_write_uint8(0x0);
    int length = husky_lens_protocol_write_end();
    protocolWrite(buffer, length);
    return true;
  }

  bool protocolReadFirmwareVersion(Protocol_t &protocol, uint8_t command)
  {
    if (husky_lens_protocol_read_begin(command))
    {
      protocol.command                = command;
      protocol.firmwareVersion.length = husky_lens_protocol_read_uint8();
      char buf[protocol.firmwareVersion.length];
      for (int i = 0; i < protocol.firmwareVersion.length; i++)
      {
        if (i > 20)
        {
          break;
        }
        protocol.firmwareVersion.data[i] = husky_lens_protocol_read_uint8();
      }
      protocol.firmwareVersion.data[20]                              = 0x00;
      protocol.firmwareVersion.data[protocol.firmwareVersion.length] = 0x00;
      husky_lens_protocol_read_end();
      return true;
    }
    else
    {
      return false;
    }
  }

  bool protocolWriteFirmwareVersion(Protocol_t &protocol, uint8_t command)
  {
    protocol.command = command;
    uint8_t *buffer  = husky_lens_protocol_write_begin(protocol.command);
    husky_lens_protocol_write_buffer_uint8(protocol.firmwareVersion.data, protocol.firmwareVersion.length);
    int length = husky_lens_protocol_write_end();
    for (int i = 0; i < protocol.firmwareVersion.length; i++)
    {
      Serial.println(protocol.firmwareVersion.data[i]);
    }
    protocolWrite(buffer, length);
    return true;
  }

  #define PROTOCOL_CREATE(function, type, command)                                                      \
    void protocolWrite##function(Protocol_t &protocol) { protocolWrite##type(protocol, command); }      \
    void protocolWrite##function()                                                                      \
    {                                                                                                   \
      Protocol_t protocol;                                                                              \
      protocolWrite##type(protocol, command);                                                           \
    }                                                                                                   \
    bool protocolRead##function(Protocol_t &protocol) { return protocolRead##type(protocol, command); } \
    bool protocolRead##function()                                                                       \
    {                                                                                                   \
      Protocol_t protocol;                                                                              \
      return protocolRead##type(protocol, command);                                                     \
    }

  PROTOCOL_CREATE(Request, Command, COMMAND_REQUEST)
  PROTOCOL_CREATE(RequestBlocks, Command, COMMAND_REQUEST_BLOCKS)
  PROTOCOL_CREATE(RequestArrows, Command, COMMAND_REQUEST_ARROWS)

  PROTOCOL_CREATE(RequestLearned, Command, COMMAND_REQUEST_LEARNED)
  PROTOCOL_CREATE(RequestBlocksLearned, Command, COMMAND_REQUEST_BLOCKS_LEARNED)
  PROTOCOL_CREATE(RequestArrowsLearned, Command, COMMAND_REQUEST_ARROWS_LEARNED)

  PROTOCOL_CREATE(RequestByID, OneInt16, COMMAND_REQUEST_BY_ID)
  PROTOCOL_CREATE(RequestBlocksByID, OneInt16, COMMAND_REQUEST_BLOCKS_BY_ID)
  PROTOCOL_CREATE(RequestArrowsByID, OneInt16, COMMAND_REQUEST_ARROWS_BY_ID)

  PROTOCOL_CREATE(ReturnInfo, FiveInt16, COMMAND_RETURN_INFO)
  PROTOCOL_CREATE(ReturnBlock, FiveInt16, COMMAND_RETURN_BLOCK)
  PROTOCOL_CREATE(ReturnArrow, FiveInt16, COMMAND_RETURN_ARROW)

  PROTOCOL_CREATE(RequestKnock, Command, COMMAND_REQUEST_KNOCK)
  PROTOCOL_CREATE(RequestAlgorithm, OneInt16, COMMAND_REQUEST_ALGORITHM)

  PROTOCOL_CREATE(ReturnOK, Command, COMMAND_RETURN_OK)

  PROTOCOL_CREATE(RequestLearn, OneInt16, COMMAND_REQUEST_LEARN)
  PROTOCOL_CREATE(RequestForget, Command, COMMAND_REQUEST_FORGET)

  PROTOCOL_CREATE(RequestSensor, FiveInt16, COMMAND_REQUEST_SENSOR)

  PROTOCOL_CREATE(RequestCustomNames, CustomNameHeader, COMMAND_REQUEST_CUSTOMNAMES)
  PROTOCOL_CREATE(RequestPhoto, Command, COMMAND_REQUEST_PHOTO)
  PROTOCOL_CREATE(RequestSendKnowledges, OneInt16, COMMAND_REQUEST_SEND_KNOWLEDGES)
  PROTOCOL_CREATE(RequestReceiveKnowledges, OneInt16, COMMAND_REQUEST_RECEIVE_KNOWLEDGES)
  PROTOCOL_CREATE(RequestCustomText, CustomTextRecv, COMMAND_REQUEST_CUSTOM_TEXT)
  PROTOCOL_CREATE(RequestClearText, Command, COMMAND_REQUEST_CLEAR_TEXT)
  PROTOCOL_CREATE(RequestSaveScreenshot, Command, COMMAND_REQUEST_SAVE_SCREENSHOT)
  PROTOCOL_CREATE(RequestIsPro, OneInt16, COMMAND_REQUEST_IS_PRO)
  PROTOCOL_CREATE(RequestFirmwareVersion, FirmwareVersion, COMMAND_REQUEST_FIRMWARE_VERSION)

  ////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////////////
};

#endif
