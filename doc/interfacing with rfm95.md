# RFM9x – STM32L476RG Hardware Connections

The **RFM9x (LoRa module)** communicates with the **STM32L476RG** via **SPI + control pins**.  
Ensure the module is powered with **3.3V only** (not 5V tolerant).

### 🔧 Suggested STM32L476RG (Nucleo Board) SPI1 Mapping

| **RFM9x Pin** | **STM32L476RG Pin** | **Function** |
|---------------|----------------------|--------------|
| VCC           | 3V3                  | Power (3.3 V only) |
| GND           | GND                  | Ground |
| SCK           | PA5                  | SPI1_SCK |
| MISO          | PA6                  | SPI1_MISO |
| MOSI          | PA7                  | SPI1_MOSI |
| NSS           | PA4                  | SPI1_NSS (can be GPIO) |
| RESET         | PB0 (GPIO)           | Reset control |
| DIO0          | PB1 (GPIO)           | IRQ for RX/TX Done |
| ANT           | Antenna connector    | LoRa antenna |

### Note
- RFM95 is essentially the SX1276 transceiver from semtech
- I-CUBE-LRWAN already supports the SX1276 reference boards (SX1276MB1MAS, SX1276MB1LAS).
- Since the RFM95 = SX1276, the LoRa HAL driver layer (radio.c, sx1276.c) is compatible.
- The difference is only in the board support package (BSP) mapping (GPIO pins: NSS, DIO0–DIO5, RESET, SPI lines).
- so we can reuse the SX1276MB1MAS / SX1276MB1LAS prebuilt project in I-CUBE-LRWAN to inteface with RFM95
- Just adapt the GPIO mapping in the board driver file (sx1276mb1mas_conf.h or sx1276mb1las_conf.h) present in application/target directory, and make sure TCXO handling is disabled (since RFM95 uses a crystal).

## how to proceed 
- import sx1276mb1mas or sx1276mb1las project to STMcubeide and alter pins and try to send a dummy packet with RFM95
- if everything is correct try to incoperate it in this repo 
