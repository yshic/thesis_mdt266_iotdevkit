/**
 * @file       bsp_uart.cpp
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Source file for bsp_uart.cpp library
 *
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_uart.h"

/* Private defines ---------------------------------------------------- */

/* Private enumerate/structure ---------------------------------------- */

/* Private macros ----------------------------------------------------- */

/* Public variables --------------------------------------------------- */
static HardwareSerial *_uart = &Serial0;

/* Private variables -------------------------------------------------- */

/* Functions definitions-------------------------------------------- */
bool bspUartSetRxTimeout(uint8_t symbols_timeout) { return _uart->setRxTimeout(symbols_timeout); }

bool bspUartSetRxFIFOFull(uint8_t fifoBytes) { return _uart->setRxFIFOFull(fifoBytes); }

bsp_uart_error_t bspUartOnReceiveError(OnReceiveErrorCb function)
{
  _uart->onReceiveError(function);
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartOnReceive(OnReceiveCb function, bool onlyOnTimeout)
{
  _uart->onReceive(function, onlyOnTimeout);
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartEventQueueReset()
{
  _uart->eventQueueReset();
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartUpdateBaudRate(unsigned long baud)
{
  _uart->updateBaudRate(baud);
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartBegin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert,
                              unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
{
  _uart->begin(baud, config, rxPin, txPin, invert, timeout_ms, rxfifo_full_thrhd);
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartEnd(void)
{
  _uart->end();
  return BSP_UART_OK;
}

int bspUartAvailable(void) { return _uart->available(); }

int bspUartAvailableForWrite(void) { return _uart->availableForWrite(); }

int bspUartPeek(void) { return _uart->peek(); }

int bspUartRead(void) { return _uart->read(); }

size_t bspUartRead(char *buffer, size_t size) { return bspUartRead((uint8_t *) buffer, size); }

size_t bspUartRead(uint8_t *buffer, size_t size) { return _uart->read(buffer, size); }

size_t bspUartReadBytes(uint8_t *buffer, size_t length) { return _uart->readBytes(buffer, length); }

size_t bspUartReadBytes(char *buffer, size_t length) { return bspUartReadBytes((uint8_t *) buffer, length); }

bsp_uart_error_t bspUartFlush(void)
{
  _uart->flush();
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartFlush(bool txOnly)
{
  _uart->flush(txOnly);
  return BSP_UART_OK;
}

size_t bspUartWrite(uint8_t c) { return _uart->write(c); }

size_t bspUartWrite(const uint8_t *buffer, size_t size) { return _uart->write(buffer, size); }

uint32_t bspUartBaudRate() { return _uart->baudRate(); }

bsp_uart_error_t bspUartSetDebugOutput(bool en)
{
  _uart->setDebugOutput(en);
  return BSP_UART_OK;
}

bsp_uart_error_t bspUartSetRxInvert(bool invert)
{
  _uart->setRxInvert(invert);
  return BSP_UART_OK;
}

bool bspUartSetPins(int8_t rxPin, int8_t txPin, int8_t ctsPin, int8_t rtsPin)
{
  return _uart->setPins(rxPin, txPin, ctsPin, rtsPin);
}

bool bspUartSetHwFlowCtrlMode(SerialHwFlowCtrl mode, uint8_t threshold)
{
  return _uart->setHwFlowCtrlMode(mode, threshold);
}

bool bspUartSetMode(SerialMode mode) { return _uart->setMode(mode); }

size_t bspUartSetRxBufferSize(size_t new_size) { return _uart->setRxBufferSize(new_size); }

size_t bspUartSetTxBufferSize(size_t new_size) { return _uart->setTxBufferSize(new_size); }

/* Private function prototypes ---------------------------------------- */

/* End of file -------------------------------------------------------- */