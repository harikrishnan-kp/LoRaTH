# LoRaTH
LoRaWAN Temperature & Humidity End Node using STM32L476RG


## Prerequisites

- **GNU Arm Embedded Toolchain**: Ensure [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) `13.3.Rel1` or newer is installed and available in your `PATH`. [***Note: Older toolchains, including version 10.x, are not supported because the linker script uses syntax accepted by newer GNU linkers.***]()
  ```bash
  # add to path
  export PATH=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin:$PATH
  
  # verify installation
  arm-none-eabi-gcc --version
  ```
- **Make** (if building with Make): Make is typically available by default on Linux systems.
- **CMake** (if building with CMake): Install CMake and ensure it is available in your `PATH`.
- **Ninja** (optional, if using CMake with Ninja generator): Ninja must be installed if you choose to build with CMake + Ninja.
  
## Build
### CMake/Ninja

The build flow uses CMake presets with the Ninja generator. Install Ninja, or add the STM32Cube bundled Ninja directory to `PATH`.

```bash
# If Ninja is installed by STM32Cube instead of the system package manager:
export PATH=$HOME/.local/share/stm32cube/bundles/ninja/1.13.1+st.1/bin:$PATH

# Configure and build Release firmware
cmake --preset Release
cmake --build --preset Release

# Configure and build Debug firmware
cmake --preset Debug
cmake --build --preset Debug
```

Build outputs are written to `build/Debug` or `build/Release`.

### Make
```bash
# Build Release firmware
make

# Build Debug firmware 
make BUILD=Debug

# Remove Make build outputs
make clean
```

Build outputs are written to `build/make/Debug` or `build/make/Release`:
`LoRaTH.elf`, `LoRaTH.bin`, `LoRaTH.hex`, and `LoRaTH.map`.

If the toolchain is not on `PATH`, prepend it to the path or pass a toolchain prefix:
```bash
make PREFIX=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
```
