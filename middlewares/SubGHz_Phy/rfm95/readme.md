I created a compact, self-contained RFM95 driver (LoRa-only) you can drop into your project and use as the transceiver driver. It follows the same register-level SPI protocol as the SX1276-based driver but focuses on the common RFM95 module usage (LoRa modem). The driver purposely leaves the low-level board / HAL hooks (SPI transfer, NSS control, reset, delay, DIO0 attach) as small stubs you must implement for your board so it integrates with your platform.

Files provided:

    rfm95.h — public API and configuration
    rfm95.c — implementation (register read/write, init, set channel, send, rx, basic config, time-on-air, RSSI, helpers)

Save both files into a folder (for example middlewares/SubGHz_Phy/rfm95) and update/include them from your project. The driver includes the SX1276 register headers already present in your repository (sx1276Regs-LoRa.h / sx1276Regs-Fsk.h), so it will reuse those definitions.

Important integration notes (read before compiling):

    You must implement the board-specific functions to control NSS, Reset, SPI, DIO0, and a millisecond delay. They are declared in the header as weak placeholders with comments where to hook in your HAL.
    This version is LoRa-only (common RFM95 use). If you need FSK, it can be extended; the SX1276 files in your repo already contain FSK registers.
    The API is named RFM95_* to avoid colliding with existing SX1276 symbols; you can adapt names if you want a drop-in replacement for SX1276 functions.
    The driver handles basic TX/RX and common configurations. For advanced features (CAD, hopping, detailed IRQ handling), integrate the DIO IRQ callbacks.

What I changed and why

    Implemented a focused LoRa driver that speaks the same SPI/register protocol used by SX1276 modules (RFM95 is a module around SX1276 family).
    Exposed a clean, small HAL boundary (five functions) that you must implement for your MCU/board — this keeps the driver portable and avoids guessing your specific SPI/HAL.
    Implemented critical features needed to initialize, set channel, send, configure RX/TX, compute time-on-air, and read RSSI. This should be enough to get basic transmission and reception up and running with an RFM95 module.
    Left hooks for DIO0 IRQ attach — recommended to use DIO0 for TXDone/RxDone interrupts for robust operation rather than polling.

Next steps for you

    Add these two files into your repo (e.g., middlewares/SubGHz_Phy/rfm95).
    Implement the board-level functions (RFM95_SPI_Transfer, RFM95_SetNss, RFM95_Reset, RFM95_Dio0Attach, RFM95_DelayMs). Example implementations depend on your HAL (STM32 HAL HAL_SPI_TransmitReceive, HAL_GPIO_WritePin, HAL_Delay, EXTI setup for DIO0).
    Include rfm95.h in your application and call RFM95_Init(), then RFM95_SetChannel(), RFM95_SetRxConfig()/SetTxConfig(), and then RFM95_Send() / RFM95_SetRx() as needed.
    If you want a drop-in replacement for the existing SX1276 driver (same function names), either rename functions or write thin wrappers that call the RFM95_* API.
