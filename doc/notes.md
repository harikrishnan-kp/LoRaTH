## icube lrwan directory structure
```
icube lrwan/
├── Applications/
|   ├── Core/ : application level codes of STM32
│   │   └── src : main entry point of program
|   |    
│   ├── LoRaWAN/ : application level codes of LoRaWAN
│   │   ├── App/
|   |   |   ├── app_lorawan.c : entry point of lora stack
|   |   |   ├── lora_app.c : main file of lora stack where everything defined 
|   |   |   
│   │   ├── Target/
|   |   |   ├── iks01a2_conf.h — sensor/shield configuration for IKS01A2 expansion (sensor pin, bus mappings). Used to adapt sensor drivers to the board/shield wiring.
|   |   |   ├── iks01a3_conf.h — sensor/shield configuration for IKS01A3 expansion (same purpose as above for IKS01A3).
|   |   |   ├── lorawan_conf.h — LoRaWAN stack configuration (regions, features, stack compile-time options). Controls which LoRaWAN middleware features/regions are compiled.
|   |   |   ├── lorawan_conf.h — LoRaWAN stack configuration (regions, features, stack compile-time options). Controls which LoRaWAN middleware features/regions are compiled.
|   |   |   ├── mw_log_conf.h — middleware logging configuration (trace levels, printf mapping) used by middleware and drivers.
|   |   |   ├── nucleo_l476rg_bus.c — board-specific bus implementations (SPI/I2C abstractions) for the NUCLEO-L476RG board.
|   |   |   ├── nucleo_l476rg_bus.h — header for the above bus implementations.
|   |   |   ├── nucleo_l476rg_errno.h — board/OS errno mapping and error helpers for NUCLEO-L476RG.
|   |   |   ├── radio_board_if.h — board-to-radio adapter: maps which radio board API to call (e.g., Sx_Board_IoInit -> SX1276MB1MAS_RADIO_IoInit) based on compile symbols (SX1276MB1MAS, SX1276MB1LAS, CMWX1ZZABZ0XX, …).
|   |   |   ├── radio_conf.h — central radio configuration header (includes platform.h, radio_board_if.h, provides RADIO_DELAY_MS, RADIO_MEMSET8, RADIO_MEMCPY8, RADIO_DIOn checks). Included by target board config headers and radio driver code to provide the board/middleware glue.
|   |   |   ├── stm32l4xx_nucleo_conf.h — Nucleo L4 board-specific definitions (LEDs, buttons, clocks).
|   |   |   ├── sx1261dvk1bas_conf.h — pin / SPI / DIO mapping for the SX1261 DVK shield (board-specific radio shield config).
|   |   |   ├── sx1262dvk1cas_conf.h — pin / SPI / DIO mapping for an SX1262 DVK shield variant.
|   |   |   ├── sx1262dvk1das_conf.h — another SX1262 DVK shield variant mapping.
|   |   |   ├── sx1272mb2das_conf.h — pin / SPI / DIO mapping for SX1272-based shield (board-specific radio shield config).
|   |   |   ├── sx1276mb1las_conf.h — pin / SPI / DIO mapping for SX1276MB1LAS daughterboard.
|   |   |   ├── sx1276mb1mas_conf.h — pin / SPI / DIO mapping for SX1276MB1MAS daughterboard.
|   |   |   ├── systime.h — system time abstraction header used by the radio stack / timers.
|   |   |   └── timer.h — timer abstraction header used by the radio/middleware (timer APIs the middleware expects).
│   │   |
│   └── SW4STM32/
|       ├── startup_stm32l476rgtx.s : startup assembly for STM32L476RGTx (vector table, initial stack pointer, reset handler, default IRQ handlers). This file sets up the CPU state on reset and branches to the C runtime entry (typically SystemInit and main). Used by SW4STM32 / STM32Cube toolchains as the MCU startup file.
|       └── syscalls.c : C file implementing low-level system call stubs (e.g., _write, _read, _sbrk, _close, _fstat, _isatty) used to retarget the C library (newlib) syscalls to the board (UART/semihosting). This lets functions like printf work on the target by routing output to a UART or semihosting channel.
|
├── Middlewares/
│   ├── LoRaWAN/
|   |   ├── Conf/
|   |   ├── Crypto/
|   |   ├── LmHandler/
|   |   ├── Mac/
|   |   |   ├── Region/
|   |   |   ├── LoRaMac.c
|   |   |   ├── LoRaMac.h
|   |   |   ├── LoRaMacAdr.c
|   |   |   ├── LoRaMacAdr.h
|   |   |   ├──
|   |   |   ├──
|   |   |   ├──
│   │   └── Utilities/
│   ├── mbedTLS/
│   │   └── ---
│   ├── SubGHZ_Phy/
│   │   ├── sx126x/ : folder containing codes for controlling sx126x radios (latest radio family by Semtech)
│   │   │   ├── radio.c : main wrapper file that implements APIs for controlling sx126x transceiver (RadioInit, RadioSetTxConfig, RadioSend, RadioRx etc).
│   │   │   ├── subghz_phy_version.h
│   │   |   ├── sx126x.h
│   │   │   └── sx126x.c
│   │   ├── sx1272/ : folder containing codes for controlling sx1272 radios.
│   │   │   ├── subghz_phy_version.h
│   │   │   ├── sx1272.c : main wrapper file that implements APIs for controlling sx1272 transceiver (RadioInit, RadioSetTxConfig, RadioSend, RadioRx etc).
│   │   │   ├── sx1272.h
│   │   │   ├── sx1272Regs-Fsk.h
│   │   │   └── sx1272Regs-LoRa.h
│   │   ├── sx1276/ : folder containing codes for controlling sx1276 radios.
│   │   │   ├── subghz_phy_version.h
│   │   │   ├── sx1276.c : main wrapper file that implements APIs for controlling sx1276 transceiver (RadioInit, RadioSetTxConfig, RadioSend, RadioRx etc).
│   │   │   ├── sx1276.h
│   │   │   ├── sx1276Regs-Fsk.h
│   │   │   └── sx1276Regs-LoRa.h
│   │   └── radio.h : main header file for application-level code (it’s only the interface). It is internally mapped to LoRa radio’s main API files (sx1272.c, sx1276.c, sx126x etc).
|
├── Drivers/
|   ├── BSP/
|   |   ├── B-L072Z-LRWAN1/ : bsp for B-L072Z-LRWAN1 board
|   |   ├── CMWX1ZZABZ_0xx/ : bsp for CMWX1ZZABZ_0xx series of LoRaWAN modules manufactured by Murata Electronics
|   |   ├── components/
|   |   |         ├── common : don`t know exactly (https://github.com/STMicroelectronics/stm32-bsp-common)
|   |   |         ├── hts221 : header files for the HTS221 humidity-temperature sensor (https://github.com/STMicroelectronics/stm32-hts221)
|   |   |         ├── lis2dw12 : header files for the LIS2DW12 accelerometer sensor
|   |   |         ├── lis2mdl : header files for the LIS2MDL magnetic sensor
|   |   |         ├── lps22hb : header files for the lps22hb pressure sensor
|   |   |         ├── lps22hh : header files for the lps22hh pressure sensor
|   |   |         ├── lps25hb : header files for the lps25hb pressure sensor
|   |   |         ├── lsm6dsl : header files for the lsm6dsl imu
|   |   |         ├── lsm6dso : header files for the lsm6dso imu
|   |   |         ├── lsm303agr : header files for the lsm303agr imu
|   |   |         └── stts751 : header files for the stts751 temperature sensor
|   |   ├── IKS01A2/ : bsp for X-NUCLEO-IKS01A2 motion MEMS and environmental sensor expansion board
|   |   ├── IKS01A3/ : bsp for X-NUCLEO-IKS01A3 motion MEMS and environmental sensor expansion board
|   |   ├── I_NUCLEO_LRWAN1/ : bsp for I-NUCLEO-LRWAN1 board
|   |   ├── LRWAN_NS1/ : bsp for LRWAN_NS1 ARDUINO® compatible expansion board
|   |   ├── MDM32L07X01/ : ?
|   |   ├── MDM32WL/ : ?
|   |   ├── STM32L0xx_Nucleo/ : bsp for STM32L0xx_Nucleo 
|   |   ├── STM32L1xx_Nucleo/ : bsp for STM32L1xx_Nucleo 
|   |   ├── STM32L4xx_Nucleo/ : bsp for STM32L4xx_Nucleo 
|   |   ├── sx1276mb1las/ : bsp for Semtech lora expansion board
|   |   ├── sx1272mb2das/ : bsp for Semtech lora expansion board
|   |   ├── sx1276mb1mas/ : bsp for Semtech lora expansion board
|   |   ├── SX1261DVK1BAS/ : bsp for Semtech lora expansion board
|   |   ├── SX1262DVK1CAS/ : bsp for Semtech lora expansion board
|   |   └── SX1262DVK1DAS/ : bsp for Semtech lora expansion board
|   ├── CMSIS/
|   ├── STM32L0xx_HAL_Driver/
|   ├── STM32L1xx_HAL_Driver/
|   ├── STM32L4xx_HAL_Driver/
|   |   ├── 

```

### note
- seems like `radio.c` in `middleware/subGHz_phy` is main file that controll all radio operations
  - but it is generated by stmcubide based on project configuration. cant see in the library
  - seems like the file is generated from the general fiel /Middlewares/Third_Party/SubGHz_Phy/sx126x/radio.c, thisfile contain the Skeleton / interface declaration (probably the original Semtech reference driver).
  - provides a generic API (RadioInit, RadioSetTxConfig, RadioSend, RadioRx) for controlling radio
  - act as a gayway between and top level application and low-level chip driver.

# Configuration
## choose frequency channel
- go to LoRaWAN/APP/lora_app.h and edit the following macros
```
/* LoraWAN application configuration (Mw is configured by lorawan_conf.h) */
#define ACTIVE_REGION                               LORAMAC_REGION_IN865
```
- you can see the implementation of ACTIVE_REGION as enum definition in `middlewares/LoRaWAN/Mac/LoRaMac.h` and the values are used inside `LoRaWAN/APP/lora_app.c`

## Select Activation type
- go to -> LoRaWAN/APP/lora_app.h and edit the following macros
```
#define LORAWAN_DEFAULT_ACTIVATION_TYPE   -> ACTIVATION_TYPE_OTAA/ACTIVATION_TYPE_ABP
```
- if you dont want to hardcode it, Add the following flag to your compiler options
```
-DLORAWAN_DEFAULT_ACTIVATION_TYPE=ACTIVATION_TYPE_ABP
```
- you can see the implementation of ACTIVATION_TYPE as enum definition in `middlewares/LoRaWAN/Mac/LoRaMac.h` and the values are used inside `LoRaWAN/APP/lora_app.c`
  - static ActivationType_t ActivationType = LORAWAN_DEFAULT_ACTIVATION_TYPE;

## Add activation keys
- **Note**: secure-element implementation supports both 1.0.x and 1.1.x LoRaWAN versions. read the doc in app/App/se-identity.h to see the changes in keys
### ABP
- Obtain the Device address, Network session key, and Application session key from Chirpstack server
- Navigate to applications -> LoRaWAN -> App -> se-identity.h and change the following macros
```
#define STATIC_DEVICE_ADDRESS    -> 1
#define LORAWAN_DEVICE_ADDRESS   -> Device address from chirpstack
#define LORAWAN_NWK_KEY          -> Network root key from Chirpstack (For LoRaWAN version 1.0.x use same key for Network root key and Network session key)
#define LORAWAN_APP_S_KEY        -> Application session key from Chirpstack
#define LORAWAN_NWK_S_KEY        -> Network session key from Chirpstack
```
### OTA
- Obtain the device EUI, join EUI, and application key from Chirpstack server
- Navigate to applications -> LoRaWAN -> App -> se-identity.h and change the following macros
```
#define STATIC_DEVICE_EUI       -> 1
#define LORAWAN_DEVICE_EUI      -> Device EUI from Chirpstack
#define LORAWAN_JOIN_EUI        -> Join EUI from Chirpstack
#define LORAWAN_APP_KEY         -> Application root key from Chirpstack
```
