TARGET := LoRaTH
BUILD ?= Debug
PREFIX ?= arm-none-eabi-

BUILD_DIR := build/make/$(BUILD)
OBJ_DIR := $(BUILD_DIR)/obj
LINKER_SCRIPT := STM32L476RGTX_FLASH.ld
ELF := $(BUILD_DIR)/$(TARGET).elf
BIN := $(BUILD_DIR)/$(TARGET).bin
HEX := $(BUILD_DIR)/$(TARGET).hex
MAP := $(BUILD_DIR)/$(TARGET).map

CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

CPU_FLAGS := -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -mcpu=cortex-m4
SPECS := --specs=nano.specs
COMMON_FLAGS := $(CPU_FLAGS) $(SPECS) -ffunction-sections -fdata-sections -Wall -fstack-usage
DEFS := -DSTM32L4 -DSTM32 -DNUCLEO_L476RG -DSTM32L476RGTx -DSTM32L476xx -DRFM95

ifeq ($(BUILD),Debug)
OPT_FLAGS := -O0 -g3
DEFS += -DDEBUG
else ifeq ($(BUILD),Release)
OPT_FLAGS := -Os
else
$(error BUILD must be Debug or Release)
endif

INCLUDE_DIRS := \
  . \
  applications/Core/Inc \
  applications/LoRaWAN/App \
  applications/LoRaWAN/Target \
  drivers/BSP/Components/Common \
  drivers/BSP/Components/hts221 \
  drivers/BSP/Components/stts751 \
  drivers/BSP/MDM32L07X01 \
  drivers/BSP/MDM32WL \
  drivers/BSP/STM32L4xx_Nucleo \
  drivers/CMSIS/Device/Include \
  drivers/CMSIS/Include \
  drivers/STM32L4xx_HAL_Driver/Inc \
  drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
  middlewares/LoRaWAN/Conf \
  middlewares/LoRaWAN/Crypto \
  middlewares/LoRaWAN/LmHandler \
  middlewares/LoRaWAN/LmHandler/Packages \
  middlewares/LoRaWAN/Mac \
  middlewares/LoRaWAN/Mac/Region \
  middlewares/LoRaWAN/Utilities \
  middlewares/SubGHz_Phy \
  middlewares/SubGHz_Phy/rfm95 \
  middlewares/SubGHz_Phy/sx126x \
  middlewares/SubGHz_Phy/sx1272 \
  middlewares/SubGHz_Phy/sx1276 \
  utilities/conf \
  utilities/lpm/tiny_lpm \
  utilities/misc \
  utilities/sequencer \
  utilities/timer \
  utilities/trace/adv_trace

INCLUDES := $(addprefix -I,$(INCLUDE_DIRS))
CFLAGS := $(COMMON_FLAGS) $(OPT_FLAGS) $(DEFS) -std=gnu11 $(INCLUDES) -MMD -MP
ASFLAGS := $(CPU_FLAGS) $(SPECS) $(OPT_FLAGS) $(DEFS) -x assembler-with-cpp $(INCLUDES) -MMD -MP
LDFLAGS := $(CPU_FLAGS) $(SPECS) -T$(LINKER_SCRIPT) --specs=nosys.specs -Wl,-Map=$(MAP) -Wl,--gc-sections -static

C_SOURCES := \
  applications/Core/Src/adc.c \
  applications/Core/Src/adc_if.c \
  applications/Core/Src/dma.c \
  applications/Core/Src/main.c \
  applications/Core/Src/rtc.c \
  applications/Core/Src/rtc_if.c \
  applications/Core/Src/stm32_lpm_if.c \
  applications/Core/Src/stm32l4xx_hal_msp.c \
  applications/Core/Src/stm32l4xx_it.c \
  applications/Core/Src/sys_app.c \
  applications/Core/Src/sys_debug.c \
  applications/Core/Src/sys_sensors.c \
  applications/Core/Src/system_stm32l4xx.c \
  applications/Core/Src/usart.c \
  applications/Core/Src/usart_if.c \
  applications/LoRaWAN/App/CayenneLpp.c \
  applications/LoRaWAN/App/app_lorawan.c \
  applications/LoRaWAN/App/lora_app.c \
  applications/LoRaWAN/App/lora_info.c \
  applications/LoRaWAN/Target/frag_decoder_if.c \
  applications/LoRaWAN/Target/nucleo_l476rg_bus.c \
  applications/LoRaWAN/Target/rfm95_board.c \
  applications/SW4STM32/syscalls.c \
  drivers/BSP/STM32L4xx_Nucleo/stm32l4xx_nucleo.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_can.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_comp.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_crc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cryp_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dac_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dcmi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dfsdm_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma2d.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dsi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_exti.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_firewall.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gfxmmu.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hash.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hash_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_hcd.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_irda.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lcd.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_lptim.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ltdc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ltdc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_mmc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_mmc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nand.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_nor.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_opamp_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_ospi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pcd_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pka.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pssi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rtc_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sai_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sd_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smartcard_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_smbus_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_sram.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_swpmi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tsc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_usart_ex.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_wwdg.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_adc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_comp.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_crc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_crs.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dac.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_dma2d.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_exti.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_fmc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_i2c.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_lptim.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_lpuart.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_opamp.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_pka.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_pwr.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rcc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rng.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_rtc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_sdmmc.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_spi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_swpmi.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_tim.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usart.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_usb.c \
  drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_utils.c \
  middlewares/LoRaWAN/Crypto/cmac.c \
  middlewares/LoRaWAN/Crypto/lorawan_aes.c \
  middlewares/LoRaWAN/Crypto/soft-se.c \
  middlewares/LoRaWAN/LmHandler/LmHandler.c \
  middlewares/LoRaWAN/LmHandler/NvmDataMgmt.c \
  middlewares/LoRaWAN/LmHandler/Packages/FragDecoder.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpClockSync.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpCompliance.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpFirmwareManagement.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpFragmentation.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpPackagesRegistration.c \
  middlewares/LoRaWAN/LmHandler/Packages/LmhpRemoteMcastSetup.c \
  middlewares/LoRaWAN/Mac/LoRaMac.c \
  middlewares/LoRaWAN/Mac/LoRaMacAdr.c \
  middlewares/LoRaWAN/Mac/LoRaMacClassB.c \
  middlewares/LoRaWAN/Mac/LoRaMacCommands.c \
  middlewares/LoRaWAN/Mac/LoRaMacConfirmQueue.c \
  middlewares/LoRaWAN/Mac/LoRaMacCrypto.c \
  middlewares/LoRaWAN/Mac/LoRaMacParser.c \
  middlewares/LoRaWAN/Mac/LoRaMacSerializer.c \
  middlewares/LoRaWAN/Mac/Region/Region.c \
  middlewares/LoRaWAN/Mac/Region/RegionAS923.c \
  middlewares/LoRaWAN/Mac/Region/RegionAU915.c \
  middlewares/LoRaWAN/Mac/Region/RegionBaseUS.c \
  middlewares/LoRaWAN/Mac/Region/RegionCN470.c \
  middlewares/LoRaWAN/Mac/Region/RegionCN779.c \
  middlewares/LoRaWAN/Mac/Region/RegionCommon.c \
  middlewares/LoRaWAN/Mac/Region/RegionEU433.c \
  middlewares/LoRaWAN/Mac/Region/RegionEU868.c \
  middlewares/LoRaWAN/Mac/Region/RegionIN865.c \
  middlewares/LoRaWAN/Mac/Region/RegionKR920.c \
  middlewares/LoRaWAN/Mac/Region/RegionRU864.c \
  middlewares/LoRaWAN/Mac/Region/RegionUS915.c \
  middlewares/LoRaWAN/Utilities/utilities.c \
  middlewares/SubGHz_Phy/sx1276/sx1276.c \
  utilities/lpm/tiny_lpm/stm32_lpm.c \
  utilities/misc/stm32_mem.c \
  utilities/misc/stm32_systime.c \
  utilities/misc/stm32_tiny_sscanf.c \
  utilities/misc/stm32_tiny_vsnprintf.c \
  utilities/sequencer/stm32_seq.c \
  utilities/timer/stm32_timer.c \
  utilities/trace/adv_trace/stm32_adv_trace.c

ASM_SOURCES := applications/SW4STM32/startup_stm32l476rgtx.s

OBJECTS := \
  $(addprefix $(OBJ_DIR)/,$(C_SOURCES:.c=.o)) \
  $(addprefix $(OBJ_DIR)/,$(ASM_SOURCES:.s=.o))

DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean size help check-toolchain

all: $(ELF) $(BIN) $(HEX)

check-toolchain:
	@command -v $(CC) >/dev/null || { echo "Missing $(CC). Install GNU Arm Embedded Toolchain or set PREFIX=/path/to/arm-none-eabi-"; exit 127; }
	@major_version=`$(CC) -dumpversion | cut -d. -f1`; \
	if [ "$$major_version" -lt 13 ]; then \
		echo "Unsupported $(CC) version $$major_version. Use GNU Arm Embedded Toolchain 13.3.Rel1 or newer."; \
		exit 1; \
	fi

$(OBJECTS): | check-toolchain

$(ELF): $(OBJECTS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

size: $(ELF)
	$(SIZE) $(ELF)

clean:
	$(RM) -r build/make

help:
	@echo "make                 Build Debug firmware"
	@echo "make BUILD=Release   Build Release firmware"
	@echo "make clean           Remove make build outputs"
	@echo "Outputs: $(BUILD_DIR)/$(TARGET).elf .bin .hex .map"

-include $(DEPS)
