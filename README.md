# LoRaTH
LoRaWAN Temperature & Humidity End Node using STM32L476RG

## Build

### Prerequisites

- **GNU Arm Embedded Toolchain**: Install [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) `13.3.Rel1` or newer and make sure it is available in your `PATH`.

  > **Note:** Older toolchains, including version 10.x, are not supported because the linker script uses syntax accepted by newer GNU linkers.

  ```bash
  # Add to PATH
  export PATH=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin:$PATH

  # Verify installation
  arm-none-eabi-gcc --version
  ```
- **Make** (if building with Make): Usually available by default on Linux systems.
- **CMake** (if building with CMake): Install CMake and ensure it is available in your `PATH`.
- **Ninja** (if building with CMake): The CMake presets use the Ninja generator, so install Ninja or add the STM32Cube bundled Ninja directory to `PATH`.

### CMake/Ninja

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

Build outputs are written to `build/Release` or `build/Debug`.

### Make

```bash
# Build Release firmware
make

# Build Debug firmware
make BUILD=Debug

# Remove Make build outputs
make clean
```

Build outputs are written to `build/make/Release` or `build/make/Debug`:
`LoRaTH.elf`, `LoRaTH.bin`, `LoRaTH.hex`, and `LoRaTH.map`.

If the toolchain is not on `PATH`, pass a toolchain prefix:
```bash
make PREFIX=<install_dir>/arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-
```