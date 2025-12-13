# RFM9x – STM32L476RG Hardware Connections

The **RFM9x (LoRa module)** communicates with the **STM32L476RG** via **SPI + control pins**.  
Ensure the module is powered with **3.3V only** (not 5V tolerant).

### 🔧 Suggested STM32L476RG (Nucleo Board) SPI1 Mapping

# STM32L476RG ↔ RFM95 Pin Connections

| **Function**      | **STM32L476RG Pin**              | **RFM95 Pin**  | **Notes**                             |
| ----------------- | -------------------------------- | -------------- | ------------------------------------- |
| **SPI_NSS (CS)**  | `PB6`                            | `NSS`          | Chip Select (active low)              |
| **SPI_SCK**       | (BUS_SPI1_SCK, usually `PA5`)    | `SCK`          | SPI clock                             |
| **SPI_MISO**      | (BUS_SPI1_MISO, usually `PA6`)   | `MISO`         | Master In Slave Out                   |
| **SPI_MOSI**      | (BUS_SPI1_MOSI, usually `PA7`)   | `MOSI`         | Master Out Slave In                   |
| **RESET**         | `PA4`                            | `RESET`        | Active low reset                      |
| **DIO0**          | `PA10`                           | `DIO0`         | Used for RxDone / TxDone interrupt    |
| **DIO1**          | `PB3`                            | `DIO1`         | Used for CAD Done, RxTimeout, etc.    |
| **DIO2**          | `PB5`                            | `DIO2`         | Optional (FHSS, CAD)                  |
| **DIO3**          | `PB4`                            | `DIO3`         | Optional                              |
| **ANT_SW**        | `PC1`                            | (Not on RFM95) | ST shield only — **ignore for RFM95** |
| **VCC**           | 3.3 V                            | `VCC`          | RFM95 is **3.3 V only**               |
| **GND**           | GND                              | `GND`          | Common ground                         |


### Note
- RFM95 is essentially the SX1276 transceiver from semtech
- I-CUBE-LRWAN already supports the SX1276 reference boards (SX1276MB1MAS, SX1276MB1LAS).
- Since the RFM95 = SX1276, the LoRa HAL driver layer (radio.c, sx1276.c) is compatible.
- The difference is only in the board support package (BSP) mapping (GPIO pins: NSS, DIO0–DIO5, RESET, SPI lines).
- so we can reuse the SX1276MB1MAS / SX1276MB1LAS prebuilt project in I-CUBE-LRWAN to inteface with RFM95
- Just adapt the GPIO mapping in the board driver file (sx1276mb1mas_conf.h or sx1276mb1las_conf.h) present in application/target directory, and make sure TCXO handling is disabled (since RFM95 uses a crystal).
- you can control activation mode (ABP/OTA), etc in Application/App/`lora_info.c`
- select your region in Applications/LoRaWAN/LoRaWAN_End_Node/LoRaWAN/Target/lorawan_conf.h

## status
✔ Firmware is running
✔ LoRaWAN stack is alive
✔ Radio TX is attempted
❌ TX never completes
    - Radio was commanded to transmit
    - Stack waited for RadioEvents->TxDone()
    - SX1276OnDio0Irq() was never called
❌ DIO0 interrupt not working
❌ Device stuck in ABP mode
❌ Gateway never receives packet

## console output
```
###### = JOINED = ABP ======================
APP_VERSION:        V1.1.0
MW_LORAWAN_VERSION: V2.3.0
MW_RADIO_VERSION:   V1.1.0
###### OTAA ######
###### AppKey:      0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x
###### NwkKey:      0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x
###### ABP  ######
###### AppSKey:     0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x
###### NwkSKey:     0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x:0x
###### DevEui:  0x:0x:0x:0x:0x:0x:0x
###### AppEui:  0x:0x:0x:0x:0x:0x:0x
###### DevAddr: 0x:0x:0x:0x:0x:0x:0x

###### = JOINED = ABP ======================
10s047:temp= 28
10s047:VDDA= 254
10s050:TX on freq 865062500 Hz at DR 0
10s055:SEND REQUEST
14s068:MAC txTimeOut

###### ========== MCPS-Confirm =============
20s047:temp= 29
20s047:VDDA= 254
20s050:TX on freq 865985000 Hz at DR 0
20s055:SEND REQUEST
24s068:MAC txTimeOut

###### ========== MCPS-Confirm =============
30s047:temp= 29
30s047:VDDA= 254
30s050:TX on freq 865062500 Hz at DR 0
30s055:SEND REQUEST
34s068:MAC txTimeOut
```
## how to proceed 
- import sx1276mb1mas or sx1276mb1las project to STMcubeide and alter pins and try to send a dummy packet with RFM95
- if everything is correct try to incoperate it in this repo 

## resources
- https://github.com/henriheimann/stm32-hal-rfm95
- https://github.com/MarcoMiglio/STM32_RFM95?tab=readme-ov-file
- https://github.com/iwanders/plainRFM95
- stm community
    - https://community.st.com/t5/stm32-mcus-wireless/interfacing-external-sensors-with-i-cube-lrwan-end-node/m-p/728390
    - https://community.st.com/t5/user/viewprofilepage/user-id/89100
    - https://community.st.com/t5/user/viewprofilepage/user-id/20806