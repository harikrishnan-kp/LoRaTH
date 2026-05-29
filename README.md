# LoRaTH
LoRaWAN Temperature & Humidity End Node using STM32L476RG

## Build
### CMake/Ninja

The build flow uses CMake presets with the Ninja generator. Install
Ninja, or add the STM32Cube bundled Ninja directory to `PATH`. Use GNU Arm
Embedded Toolchain `13.3.Rel1` or newer. Older toolchains, including version
10.x, are not supported because the linker script uses syntax accepted by newer
GNU linkers.

```bash
export PATH=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin:$PATH

# If Ninja is installed by STM32Cube instead of the system package manager:
export PATH=$HOME/.local/share/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH

# Configure and build Debug firmware
cmake --preset Debug
cmake --build --preset Debug

# Configure and build Release firmware
cmake --preset Release
cmake --build --preset Release
```

Build outputs are written to `build/Debug` or `build/Release`.

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

If the toolchain is not on `PATH`, prepend it to the path:

```bash
export PATH=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin:$PATH
arm-none-eabi-gcc --version
```
or pass a toolchain prefix:
```bash
make PREFIX=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
```
