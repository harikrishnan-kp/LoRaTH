#include "rfm95.h"
#include <string.h> /* for memset */
#include <math.h>   /* for ceil in ToA calc — if not available you can simplify */

#define REG_OPMODE            REG_LR_OPMODE
#define REG_FRF_MSB           REG_LR_FRFMSB
#define REG_FRF_MID           REG_LR_FRFMID
#define REG_FRF_LSB           REG_LR_FRFLSB
#define REG_PA_CONFIG         REG_PACONFIG
#define REG_FIFO_ADDR_PTR     REG_LR_FIFOADDRPTR
#define REG_FIFO_RX_BASE_ADDR REG_LR_FIFORXBASEADDR
#define REG_FIFO_TX_BASE_ADDR REG_LR_FIFOTXBASEADDR
#define REG_FIFO               REG_LR_FIFO
#define REG_IRQ_FLAGS         REG_LR_IRQFLAGS
#define REG_PAYLOAD_LENGTH     REG_LR_PAYLOADLENGTH
#define REG_MODEM_CONFIG1      REG_LR_MODEMCONFIG1
#define REG_MODEM_CONFIG2      REG_LR_MODEMCONFIG2
#define REG_MODEM_CONFIG3      REG_LR_MODEMCONFIG3
#define REG_SYMB_TIMEOUT_LSB   REG_LR_SYMBTIMEOUTLSB
#define REG_PREAMBLE_MSB       REG_LR_PREAMBLEMSB
#define REG_PREAMBLE_LSB       REG_LR_PREAMBLELSB
#define REG_DETECT_OPTIMIZE    REG_LR_DETECTOPTIMIZE
#define REG_DETECTION_THRESHOLD REG_LR_DETECTIONTHRESHOLD
#define REG_RSSI_VALUE_LORA    REG_LR_RSSIVALUE
#define REG_PKT_RSSI_VALUE     REG_LR_PKTRSSIVALUE
#define REG_PKT_SNR_VALUE      REG_LR_PKTSNRVALUE
#define REG_HOP_PERIOD         REG_LR_HOPPERIOD
#define REG_INVERTIQ           REG_LR_INVERTIQ

/* Internal state */
static uint32_t rfm95_wakeup_time = RFM95_WAKEUP_TIME;
static uint32_t current_frequency = 0;
static uint8_t max_payload_len = 255;

/* Low-level SPI register access */

/* Write single register (MSB=1 for write on SX127x family) */
void RFM95_WriteReg(uint8_t addr, uint8_t data)
{
    uint8_t buf[2];
    buf[0] = addr | 0x80;
    buf[1] = data;
    RFM95_SetNss(true); /* NSS low */
    RFM95_SPI_Transfer(buf, NULL, 2);
    RFM95_SetNss(false); /* NSS high */
}

/* Read single register (MSB=0 for read) */
uint8_t RFM95_ReadReg(uint8_t addr)
{
    uint8_t tx[2], rx[2];
    tx[0] = addr & 0x7F;
    tx[1] = 0x00;
    RFM95_SetNss(true);
    RFM95_SPI_Transfer(tx, rx, 2);
    RFM95_SetNss(false);
    return rx[1];
}

void RFM95_WriteBuffer(uint8_t addr, uint8_t *buffer, uint8_t size)
{
    uint8_t tx[size + 1];
    tx[0] = addr | 0x80;
    memcpy(&tx[1], buffer, size);
    RFM95_SetNss(true);
    RFM95_SPI_Transfer(tx, NULL, size + 1);
    RFM95_SetNss(false);
}

void RFM95_ReadBuffer(uint8_t addr, uint8_t *buffer, uint8_t size)
{
    /* send addr then read size bytes */
    uint8_t tx[size + 1];
    uint8_t rx[size + 1];
    tx[0] = addr & 0x7F;
    memset(&tx[1], 0x00, size);
    RFM95_SetNss(true);
    RFM95_SPI_Transfer(tx, rx, size + 1);
    RFM95_SetNss(false);
    memcpy(buffer, &rx[1], size);
}

/* Helpers */
static void rfm95_set_mode(uint8_t mode)
{
    /* mode is raw OPMODE bits (sleep/standby/tx/rx) */
    RFM95_WriteReg(REG_OPMODE, (RFM95_ReadReg(REG_OPMODE) & 0xF8) | (mode & 0x07));
    /* note: additional bits might be required if using LoRa/RXCONT etc. */
}

void RFM95_SetSleep(void)
{
    /* LoRa sleep: set LoRa bit and sleep mode */
    RFM95_WriteReg(REG_OPMODE, 0x00 | RF_OPMODE_LONGRANGEMODE_ON | RF_OPMODE_MODE_SLEEP);
}

void RFM95_SetStby(void)
{
    RFM95_WriteReg(REG_OPMODE, RF_OPMODE_LONGRANGEMODE_ON | RF_OPMODE_MODE_STDBY);
}

void RFM95_SetTx(uint32_t timeout_ms)
{
    (void)timeout_ms;
    RFM95_WriteReg(REG_OPMODE, RF_OPMODE_LONGRANGEMODE_ON | RF_OPMODE_MODE_TRANSMITTER);
}

void RFM95_SetRx(uint32_t timeout_ms)
{
    (void)timeout_ms;
    /* continuous receive */
    RFM95_WriteReg(REG_OPMODE, RF_OPMODE_LONGRANGEMODE_ON | RF_OPMODE_MODE_RECEIVER);
}

/* Public API */

uint32_t RFM95_Init(void)
{
    /* Board reset */
    RFM95_Reset();
    RFM95_DelayMs(10);
    /* Default setup: LoRa mode, standby, set FIFO base addresses */
    RFM95_SetSleep();
    RFM95_DelayMs(1);

    /* Set FIFO base addresses */
    RFM95_WriteReg(REG_FIFO_TX_BASE_ADDR, 0x00);
    RFM95_WriteReg(REG_FIFO_RX_BASE_ADDR, 0x00);

    /* Set LORA modem parameters to safe defaults:
       - BW = 125 kHz (value 0), CR = 4/5 (1), SF = 7 (7)
       - CRC on by default
    */
    /* ModemConfig1: BW(4bits)|CR(3bits)|ImplicitHeaderModeOn(1bit) */
    RFM95_WriteReg(REG_MODEM_CONFIG1, (0 << 4) | (1 << 1) | (0));
    /* ModemConfig2: SF(4bits)|TxContinuousMode(1)|RxPayloadCrcOn(1) */
    RFM95_WriteReg(REG_MODEM_CONFIG2, (7 << 4) | (0 << 3) | (1 << 2));
    /* ModemConfig3: LowDataRateOptimize(1)|AgcAutoOn(1) on */
    RFM95_WriteReg(REG_MODEM_CONFIG3, (0 << 3) | (1 << 2));

    /* PA config: use default PA_BOOST path if available, 17 dBm by default */
    RFM95_WriteReg(REG_PA_CONFIG, 0x80 | 0x70); /* PA_BOOST + OutputPower */

    /* Set public network sync word */
    RFM95_SetPublicNetwork(true);

    /* Max payload */
    max_payload_len = 255;
    RFM95_WriteReg(REG_PAYLOAD_LENGTH, max_payload_len);

    RFM95_SetStby();
    return rfm95_wakeup_time;
}

void RFM95_SetChannel(uint32_t freq)
{
    current_frequency = freq;
    uint64_t frf = ((uint64_t)freq << 19) / 32000000;
    RFM95_WriteReg(REG_FRF_MSB, (uint8_t)((frf >> 16) & 0xFF));
    RFM95_WriteReg(REG_FRF_MID, (uint8_t)((frf >> 8) & 0xFF));
    RFM95_WriteReg(REG_FRF_LSB, (uint8_t)(frf & 0xFF));
}

void RFM95_SetTxConfig(int8_t power, uint32_t bandwidth, uint32_t datarate,
                      uint8_t coderate, uint16_t preambleLen,
                      bool fixLen, bool crcOn, bool freqHopOn, uint8_t hopPeriod,
                      bool iqInverted, uint32_t timeout)
{
    (void)bandwidth;
    (void)datarate;
    (void)coderate;
    (void)preambleLen;
    (void)fixLen;
    (void)crcOn;
    (void)freqHopOn;
    (void)hopPeriod;
    (void)iqInverted;
    (void)timeout;
    /* Power handling: map power to PA config (simple approach) */
    uint8_t paConfig;
    if (power < 2) power = 2;
    if (power > 17) power = 17;
    paConfig = 0x80 | (uint8_t)(power & 0x0F);
    RFM95_WriteReg(REG_PA_CONFIG, paConfig);
}

void RFM95_SetRxConfig(uint32_t bandwidth, uint32_t datarate, uint8_t coderate,
                       uint16_t preambleLen, uint16_t symbTimeout,
                       bool fixLen, uint8_t payloadLen, bool crcOn,
                       bool freqHopOn, uint8_t hopPeriod, bool iqInverted,
                       bool rxContinuous)
{
    (void)freqHopOn;
    (void)hopPeriod;
    (void)rxContinuous;

    /* Map bandwidth value (Hz) to register value:
       125kHz -> 0, 250kHz -> 1, 500kHz -> 2
       If caller gives one of these speeds in Hz, map accordingly, else assume enumeration value.
    */
    uint8_t bw_reg = 0;
    if (bandwidth == 125000) bw_reg = 0;
    else if (bandwidth == 250000) bw_reg = 1;
    else if (bandwidth == 500000) bw_reg = 2;
    else bw_reg = (uint8_t)bandwidth; /* if they pass enum-like values */

    uint8_t sf_reg = (uint8_t)datarate; /* expects LoRa SF number (6..12) */
    uint8_t cr_reg = (coderate & 0x07); /* expects 1..4 for 4/5..4/8 -> code in callers may differ */

    /* Set ModemConfig1 */
    uint8_t modem1 = (bw_reg << 4) | ((cr_reg & 0x07) << 1) | (fixLen ? 1 : 0);
    RFM95_WriteReg(REG_MODEM_CONFIG1, modem1);

    /* ModemConfig2: SF | RxPayloadCrcOn |
       set SymbTimeout (MSB+LSB) */
    uint8_t modem2 = (sf_reg << 4) | ((crcOn ? 1 : 0) << 2);
    RFM95_WriteReg(REG_MODEM_CONFIG2, modem2);

    RFM95_WriteReg(REG_PREAMBLE_MSB, (uint8_t)((preambleLen >> 8) & 0xFF));
    RFM95_WriteReg(REG_PREAMBLE_LSB, (uint8_t)(preambleLen & 0xFF));

    /* Symb timeout LSB */
    RFM95_WriteReg(REG_SYMB_TIMEOUT_LSB, (uint8_t)(symbTimeout & 0xFF));

    /* Payload length for implicit header mode must match length; caller passes payloadLen */
    if (fixLen)
    {
        RFM95_WriteReg(REG_PAYLOAD_LENGTH, payloadLen);
    }
}

void RFM95_Send(uint8_t *buffer, uint8_t size)
{
    if (size > max_payload_len) size = max_payload_len;
    /* Set FIFO pointers */
    RFM95_WriteReg(REG_FIFO_ADDR_PTR, 0x00);
    /* Write payload to FIFO */
    RFM95_WriteBuffer(REG_FIFO, buffer, size);
    /* Set payload length */
    RFM95_WriteReg(REG_PAYLOAD_LENGTH, size);
    /* Clear IRQ flags */
    RFM95_WriteReg(REG_IRQ_FLAGS, 0xFF);
    /* Start TX */
    RFM95_SetTx(0);
    /* Wait for TXDone — user can instead attach DIO0 IRQ to be notified */
    /* Busy-wait basic approach (polling IRQ flag) */
    while ((RFM95_ReadReg(REG_IRQ_FLAGS) & RF_LR_IRQFLAGS_TXDONE) == 0)
    {
        /* crude delay */
        RFM95_DelayMs(1);
    }
    /* Clear TXDone */
    RFM95_WriteReg(REG_IRQ_FLAGS, RF_LR_IRQFLAGS_TXDONE);
    RFM95_SetStby();
}

void RFM95_SetMaxPayloadLength(uint8_t max)
{
    max_payload_len = max;
    RFM95_WriteReg(REG_PAYLOAD_LENGTH, max);
}

uint32_t RFM95_GetTimeOnAir(uint32_t bandwidth, uint32_t datarate, uint8_t coderate,
                           uint16_t preambleLen, bool fixLen, uint8_t payloadLen, bool crcOn)
{
    /* Time-on-air calculation based on Semtech LoRa formula.
       This is an approximate implementation — if no math.h, replace ceil calls.
    */
    double bw_hz = (double)bandwidth; /* caller should pass 125000/250000/500000 */
    double sf = (double)datarate;
    double cr = (double)coderate; /* expects 1..4 -> denominator 4/(4+cr-1) mapping may be needed */
    double crc = crcOn ? 1.0 : 0.0;
    double H = fixLen ? 1.0 : 0.0; /* 0 => explicit, 1 => implicit in formula use */
    double payloadSymbNb;
    double ts; /* symbol duration in ms */
    double tPreamble;
    double tPayload;
    /* symbol duration (ms) = 1000 * 2^SF / BW */
    ts = 1000.0 * pow(2.0, sf) / bw_hz;
    tPreamble = (preambleLen + 4.25) * ts;
    /* effective coding rate */
    double codingRate = 4.0 / (4.0 + (cr - 1.0)); /* if coderate supplied as 1..4 */
    /* payload symbol number */
    double tmp = ceil((8.0 * payloadLen - 4.0 * sf + 28.0 + 16.0 * crc - 20.0 * H) /
                      (4.0 * (sf - 2.0 * (bandwidth == 125000 ? 1 : 0))) ) * (cr);
    /* fallback simpler approximation if above becomes NaN */
    if (!isfinite(tmp) || tmp < 0) tmp = 0;
    payloadSymbNb = 8.0 + tmp;
    tPayload = payloadSymbNb * ts;
    return (uint32_t)ceil(tPreamble + tPayload);
}

int16_t RFM95_ReadRssi(void)
{
    /* LoRa RSSI reading: read REG_PKT_RSSI_VALUE and scale for frequency band */
    int16_t rssi = (int16_t)RFM95_ReadReg(REG_PKT_RSSI_VALUE);
    rssi = -157 + rssi; /* typical conversion for SX1276 LoRa */
    return rssi;
}

uint32_t RFM95_Random(void)
{
    /* Build 32-bit random using RSSI register variations */
    uint32_t rnd = 0;
    RFM95_SetSleep();
    RFM95_WriteReg(REG_OPMODE, RF_OPMODE_LONGRANGEMODE_ON | RF_OPMODE_MODE_RECEIVER);
    RFM95_DelayMs(1);
    for (int i = 0; i < 32; i += 8)
    {
        uint8_t v = RFM95_ReadReg(REG_RSSI_VALUE_LORA);
        rnd |= ((uint32_t)v) << i;
        RFM95_DelayMs(1);
    }
    RFM95_SetStby();
    return rnd;
}

bool RFM95_IsChannelFree(uint32_t freq, uint32_t rxBandwidth, int16_t rssiThresh, uint32_t maxCarrierSenseTime)
{
    (void)freq;
    (void)rxBandwidth;
    uint32_t t = 0;
    RFM95_SetRx(0);
    while (t < maxCarrierSenseTime)
    {
        int16_t rssi = RFM95_ReadRssi();
        if (rssi > rssiThresh)
        {
            RFM95_SetStby();
            return false;
        }
        RFM95_DelayMs(1);
        t++;
    }
    RFM95_SetStby();
    return true;
}

void RFM95_SetPublicNetwork(bool enable)
{
    uint8_t sync = enable ? LORA_MAC_PUBLIC_SYNCWORD : LORA_MAC_PRIVATE_SYNCWORD;
    RFM95_WriteReg(REG_LR_SYNCWORD, sync);
}

uint32_t RFM95_GetWakeupTime(void)
{
    return rfm95_wakeup_time;
}

/* End of rfm95.c */