#ifndef __RFM95_H__
#define __RFM95_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* Use existing register definitions from the SX1276 headers in your tree */
#include "sx1276Regs-LoRa.h"
#include "sx1276Regs-Fsk.h"

/* Radio wake-up time from sleep (ms) */
#define RFM95_WAKEUP_TIME 2U

/* Public API - LoRa only driver (basic) */

/* Board/HAL integration hooks — you must implement these for your board.
   Provide implementations in your board file (e.g., board_rfm95.c).
   The prototypes are intentionally simple:
   - RFM95_SPI_Transfer: perform full-duplex SPI transfer of count bytes.
     tx buffer may be NULL to send 0x00; rx buffer may be NULL if rx unused.
   - RFM95_SetNss(bool level): set NSS (chip select) low(true) or high(false).
   - RFM95_Reset(): pulse reset line (active low).
   - RFM95_Dio0Attach(void (*cb)(void)): attach DIO0 rising-edge IRQ callback.
   - RFM95_DelayMs(ms): blocking millisecond delay.
*/
void RFM95_SPI_Transfer(uint8_t *tx, uint8_t *rx, uint16_t count);
void RFM95_SetNss(bool level);
void RFM95_Reset(void);
void RFM95_Dio0Attach(void (*callback)(void));
void RFM95_DelayMs(uint32_t ms);

/* Initialization and state */
uint32_t RFM95_Init(void); /* returns wakeup time in ms */

/* Basic mode control */
void RFM95_SetSleep(void);
void RFM95_SetStby(void);
void RFM95_SetTx(uint32_t timeout_ms);
void RFM95_SetRx(uint32_t timeout_ms);

/* RF config */
void RFM95_SetChannel(uint32_t freq);
void RFM95_SetTxConfig(int8_t power, uint32_t bandwidth, uint32_t datarate,
                      uint8_t coderate, uint16_t preambleLen,
                      bool fixLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                      bool iqInverted, uint32_t timeout);
void RFM95_SetRxConfig(uint32_t bandwidth, uint32_t datarate, uint8_t coderate,
                       uint16_t preambleLen, uint16_t symbTimeout,
                       bool fixLen, uint8_t payloadLen, bool crcOn,
                       bool freqHopOn, uint8_t hopPeriod, bool iqInverted,
                       bool rxContinuous);

/* Packet operations */
void RFM95_Send(uint8_t *buffer, uint8_t size);
void RFM95_SetMaxPayloadLength(uint8_t max);
uint32_t RFM95_GetTimeOnAir(uint32_t bandwidth, uint32_t datarate, uint8_t coderate,
                           uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn);

/* Utility */
int16_t RFM95_ReadRssi(void);
uint32_t RFM95_Random(void);
bool RFM95_IsChannelFree(uint32_t freq, uint32_t rxBandwidth, int16_t rssiThresh, uint32_t maxCarrierSenseTime);
void RFM95_SetPublicNetwork(bool enable);
uint32_t RFM95_GetWakeupTime(void);

/* Low-level register helpers (exposed for advanced use) */
void RFM95_WriteReg(uint8_t addr, uint8_t data);
uint8_t RFM95_ReadReg(uint8_t addr);
void RFM95_WriteBuffer(uint8_t addr, uint8_t *buffer, uint8_t size);
void RFM95_ReadBuffer(uint8_t addr, uint8_t *buffer, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* __RFM95_H__ */