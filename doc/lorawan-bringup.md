# LoRaWAN Bring-Up

This firmware is set for first uplink bring-up:

- Serial logs: USART2 on the ST-LINK virtual COM port, `115200 8N1`.
- Debugging: SWD is kept enabled and stop-mode low power is disabled.
- Uplink period: one dummy frame every `10000 ms`.
- Payload port: `2`.
- Payload format: STM32 LoRaWAN example payload, copied from the NUCLEO-L476RG I-CUBE-LRWAN end-node example.
- Activation: ABP while `LORATH_BRINGUP_USE_ABP` is `1` in `applications/LoRaWAN/App/lora_app.h`.

Register the same ABP values on the LoRaWAN server:

- DevAddr: `0x0100000A`
- NwkSKey: `2B7E151628AED2A6ABF7158809CF4F3C`
- AppSKey: `2B7E151628AED2A6ABF7158809CF4F3C`

The active region is currently `LORAMAC_REGION_IN865` in `applications/LoRaWAN/App/lora_app.h`, and only `REGION_IN865` is enabled in `applications/LoRaWAN/Target/lorawan_conf.h`. The gateway, server device profile, and firmware region must all match.

Expected serial log flow:

```text
APP_VERSION: ...
MW_LORAWAN_VERSION: ...
MW_RADIO_VERSION: ...
LoRaWAN bring-up: region=... activation=ABP tx_period=10000ms
###### = JOINED = ABP
STM32 example uplink request: port=2 temp=... pressure=... humidity=... battery=... size=15
SEND REQUEST accepted by LoRaWAN stack
###### ========== MCPS-Confirm =============
```

After the server receives dummy frames, replace the placeholder keys before using the device outside a private lab setup.

## Debugging Commands

### Find the serial console

The NUCLEO ST-LINK virtual COM port usually appears as `/dev/ttyACM0`.

```bash
ls -l /dev/serial/by-id
ls -l /dev/ttyACM0 /dev/ttyACM1 /dev/ttyUSB0 /dev/ttyUSB1
```

Expected device link:

```text
usb-STMicroelectronics_STM32_STLink_...-if02 -> ../../ttyACM0
```

If no `/dev/ttyACM*` appears, unplug/replug the board and check:

```bash
lsusb
```

Expected USB device:

```text
STMicroelectronics ST-LINK/V2.1
```

### Read firmware logs

Configure the port:

```bash
stty -F /dev/ttyACM0 115200 cs8 -cstopb -parenb -ixon -ixoff -echo raw
```

Capture logs for 60 seconds:

```bash
timeout 60s cat /dev/ttyACM0
```

Or use `screen` interactively:

```bash
screen /dev/ttyACM0 115200
```

Exit `screen` with:

```text
Ctrl+A, then \, then y
```

If that does not work, in another terminal:

```bash
screen -ls
screen -S <session-id> -X quit
```

### Build firmware

```bash
cmake --build --preset Debug
```

Successful build ends with:

```text
Linking C executable LoRaTH.elf
```

### Flash firmware

```bash
/home/hari/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI \
  -c port=SWD mode=UR reset=HWrst \
  -w build/Debug/LoRaTH.elf \
  -v \
  -rst
```

Successful flash includes:

```text
Download verified successfully
MCU Reset
```

### Reset the MCU

```bash
/home/hari/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI \
  -c port=SWD mode=UR reset=HWrst \
  -rst
```

This is useful when `cat /dev/ttyACM0` is already running and you want to capture boot logs from the beginning.

### Stop stuck debug sessions

List active debug processes:

```bash
ps aux | grep -E "STLinkDebugTargetAdapter|ST-LINK_gdbserver|st-util|openocd|arm-none-eabi-gdb|gdb" | grep -v grep
```

Stop them:

```bash
pkill -f STLinkDebugTargetAdapter
pkill -f ST-LINK_gdbserver
pkill -f arm-none-eabi-gdb
pkill -f "cube arm-none-eabi-gdb"
pkill -f openocd
pkill -f st-util
```

Then unplug/replug the board.

## Reading The Logs

Good boot and ABP join:

```text
APP_VERSION:        V1.1.0
MW_LORAWAN_VERSION: V2.3.0
MW_RADIO_VERSION:   V1.1.0
LoRaWAN bring-up: region=7 activation=ABP tx_period=10000ms
###### = JOINED = ABP ======================
```

Good application send request:

```text
STM32 example uplink request: port=2 temp=38 pressure=10000 humidity=500 battery=254 size=15
TX on freq 865062500 Hz at DR 0
SEND REQUEST accepted by LoRaWAN stack
```

`region=7` means `LORAMAC_REGION_IN865`.

`TX on freq 865062500 Hz` or `865985000 Hz` means the stack is using Indian IN865 uplink channels.

### Important failure messages

```text
MAC txTimeOut
```

The stack started a radio transmission but did not receive the radio `TxDone` interrupt. Check the RFM95 wiring, especially `DIO0`, because SX1276/RFM95 normally reports `TxDone` on DIO0.

Expected RFM95 wiring for this project:

```text
SPI1 SCK  PA5
SPI1 MISO PA6
SPI1 MOSI PA7
NSS       PB6
RESET     PA4
DIO0      PA10
DIO1      PB3
DIO2      PB5
DIO3      PB4
GND       common ground
3.3V      stable supply
```

Repeated only this line:

```text
APP_VERSION:        V1.1.0
```

This usually means the firmware reached logging but then got stuck in an interrupt/default handler during init. We already fixed known DMA and RTC handler name mismatches in `applications/Core/Src/stm32l4xx_it.c`.

No serial output:

- Make sure the correct port is `/dev/ttyACM0`.
- Make sure no old `screen`, GDB, or Cube debug session is holding the port.
- Reset the MCU while `timeout 60s cat /dev/ttyACM0` is already running.
- Check that `APP_LOG_ENABLED` is `1` in `applications/Core/Inc/sys_conf.h`.

## Server Checklist

For ABP, server values must match firmware exactly:

```text
Region:  IN865
DevAddr: 0100000A
NwkSKey: 2B7E151628AED2A6ABF7158809CF4F3C
AppSKey: 2B7E151628AED2A6ABF7158809CF4F3C
FCnt validation: relaxed/disabled during bring-up if the server supports it
```

If the firmware logs show repeated uplink attempts but the server sees nothing, check:

- Gateway is online and configured for IN865.
- Gateway antenna is attached and close enough for bench testing.
- RFM95 module is the correct frequency variant for 865 MHz operation.
- RFM95 `DIO0` is wired to `PA10`.
- RFM95 is powered from stable 3.3 V, not 5 V.

## Current Payload Format

The current `SendTxData()` implementation follows the original STM32 I-CUBE-LRWAN NUCLEO-L476RG example from:

```text
/home/hari/Downloads/en.i-cube_lrwan/STM32CubeExpansion_LRWAN_V2.1.0/Projects/NUCLEO-L476RG/Applications/LoRaWAN/LoRaWAN_End_Node/LoRaWAN/App/lora_app.c
```

With `CAYENNE_LPP` disabled and `SENSOR_ENABLED` set to `0`, the uplink payload is 15 bytes:

```text
byte 0      AppLedStateOn
bytes 1-2  pressure, big endian, hPa / 10
byte 3     temperature, signed low byte, deg C
bytes 4-5  humidity, big endian, percent * 10
byte 6     battery level, 1..254
bytes 7-9  latitude, signed 24-bit
bytes 10-12 longitude, signed 24-bit
bytes 13-14 altitude, big endian
```

Example from the verified serial log:

```text
STM32 example uplink request: port=2 temp=38 pressure=10000 humidity=500 battery=254 size=15
```

That corresponds approximately to this payload:

```text
00 27 10 26 01 F4 FE 00 00 00 00 00 00 00 00
```
