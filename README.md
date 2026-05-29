# LoRaTH
LoRaWAN Temperature & Humidity End Node using STM32L476RG

## Build
### Make build

The project supports building using GNU Make and the GNU Arm Embedded Toolchain. The GNU Arm Embedded Toolchain can be downloaded from [link](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads). Use Toolchain version `13.3.Rel1` or newer. Older toolchains, including version 10.x, are not supported because the linker script uses syntax accepted by newer GNU linkers.

```bash
# Build Debug firmware 
make

# Build Release firmware
make BUILD=Release

# Remove Make build outputs
make clean
```

Build outputs are written to `build/make/Debug` or `build/make/Release`:
`LoRaTH.elf`, `LoRaTH.bin`, `LoRaTH.hex`, and `LoRaTH.map`.

If the toolchain is not on `PATH`, append it to the PATH:

```bash
export PATH=$PATH:<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin
arm-none-eabi-gcc --version
```
or pass a toolchain prefix:
```bash
make PREFIX=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
```
