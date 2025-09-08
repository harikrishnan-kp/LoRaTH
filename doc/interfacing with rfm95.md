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

