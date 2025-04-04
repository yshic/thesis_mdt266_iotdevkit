/**
 * @file       bsp_uart.h
 * @license    This library is released under the MIT License.
 * @version    0.1.0
 * @date       2025-03-29
 * @author     Tuan Nguyen
 *
 * @brief      Header file for BSP_UART library
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_UART_H
  #define BSP_UART_H

  /* Includes ----------------------------------------------------------- */
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif

  /* Public defines ----------------------------------------------------- */
  #define BSP_UART_LIB_VERSION (F("0.1.0"))

/* Public enumerate/structure ----------------------------------------- */

// Error codes for UART
typedef enum
{
  BSP_UART_OK = 0,
  BSP_UART_ERR,
  BSP_UART_ERR_READ,
  BSP_UART_ERR_WRITE,
  BSP_UART_TIMEOUT
} bsp_uart_error_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Functions Declaration -------------------------------------------------- */
bool bspUartSetRxTimeout(uint8_t symbols_timeout);

bool bspUartSetRxFIFOFull(uint8_t fifoBytes);

bsp_uart_error_t bspUartOnReceive(OnReceiveCb function, bool onlyOnTimeout = false);

bsp_uart_error_t bspUartOnReceiveError(OnReceiveErrorCb function);

bsp_uart_error_t bspUartEventQueueReset();

bsp_uart_error_t bspUartUpdateBaudRate(unsigned long baud);

bsp_uart_error_t bspUartBegin(unsigned long baud, uint32_t config = SERIAL_8N1, int8_t rxPin = -1,
                              int8_t txPin = -1, bool invert = false, unsigned long timeout_ms = 20000UL,
                              uint8_t rxfifo_full_thrhd = 112);

bsp_uart_error_t bspUartEnd(void);

int bspUartAvailable(void);

int bspUartAvailableForWrite(void);

int bspUartPeek(void);

int bspUartRead(void);

size_t bspUartRead(uint8_t *buffer, size_t size);

size_t bspUartRead(char *buffer, size_t size);

size_t bspUartReadBytes(uint8_t *buffer, size_t length);

size_t bspUartReadBytes(char *buffer, size_t length);

bsp_uart_error_t bspUartFlush(void);

bsp_uart_error_t bspUartFlush(bool txOnly);

size_t bspUartWrite(uint8_t);

size_t bspUartWrite(const uint8_t *buffer, size_t size);

inline size_t bspUartWrite(const char *buffer, size_t size) { return bspUartWrite((uint8_t *) buffer, size); }

inline size_t bspUartWrite(const char *s) { return bspUartWrite((uint8_t *) s, strlen(s)); }

inline size_t bspUartWrite(unsigned long n) { return bspUartWrite((uint8_t) n); }

inline size_t bspUartWrite(long n) { return bspUartWrite((uint8_t) n); }

inline size_t bspUartWrite(unsigned int n) { return bspUartWrite((uint8_t) n); }

inline size_t bspUartWrite(int n) { return bspUartWrite((uint8_t) n); }

uint32_t bspUartBaudRate();

bsp_uart_error_t bspUartSetDebugOutput(bool);

bsp_uart_error_t bspUartSetRxInvert(bool);

bool bspUartSetPins(int8_t rxPin, int8_t txPin, int8_t ctsPin = -1, int8_t rtsPin = -1);

bool bspUartSetHwFlowCtrlMode(SerialHwFlowCtrl mode      = UART_HW_FLOWCTRL_CTS_RTS,
                              uint8_t          threshold = 64); // 64 is half FIFO Length

bool bspUartSetMode(SerialMode mode);

size_t bspUartSetRxBufferSize(size_t new_size);

size_t bspUartSetTxBufferSize(size_t new_size);

#endif // BSP_UART_H

/* End of file -------------------------------------------------------- */