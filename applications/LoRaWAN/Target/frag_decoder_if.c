/**
  ******************************************************************************
  * @file    frag_decoder_if.c
  * @brief   Application storage hooks for the LoRaWAN fragmentation decoder.
  ******************************************************************************
  */

#include "frag_decoder_if.h"
#include "utilities.h"

static uint8_t FragDecoderStorage[FRAG_MAX_NB * FRAG_MAX_SIZE];

static int32_t FragDecoderErase(void);
static int32_t FragDecoderWrite(uint32_t addr, uint8_t *data, uint32_t size);
static int32_t FragDecoderRead(uint32_t addr, uint8_t *data, uint32_t size);
static void FragDecoderOnProgress(uint16_t fragCounter, uint16_t fragNb, uint8_t fragSize, uint16_t fragNbLost);
static void FragDecoderOnDone(int32_t status, uint32_t size);

LmhpFragmentationParams_t FRAG_DECODER_IF_FragmentationParams =
{
  .DecoderCallbacks =
  {
    .FragDecoderErase = FragDecoderErase,
    .FragDecoderWrite = FragDecoderWrite,
    .FragDecoderRead = FragDecoderRead,
  },
  .OnProgress = FragDecoderOnProgress,
  .OnDone = FragDecoderOnDone,
};

static int32_t FragDecoderErase(void)
{
  UTIL_MEM_set_8(FragDecoderStorage, 0, sizeof(FragDecoderStorage));
  return 0;
}

static int32_t FragDecoderWrite(uint32_t addr, uint8_t *data, uint32_t size)
{
  if ((addr + size) > sizeof(FragDecoderStorage))
  {
    return -1;
  }

  UTIL_MEM_cpy_8(&FragDecoderStorage[addr], data, size);
  return 0;
}

static int32_t FragDecoderRead(uint32_t addr, uint8_t *data, uint32_t size)
{
  if ((addr + size) > sizeof(FragDecoderStorage))
  {
    return -1;
  }

  UTIL_MEM_cpy_8(data, &FragDecoderStorage[addr], size);
  return 0;
}

static void FragDecoderOnProgress(uint16_t fragCounter, uint16_t fragNb, uint8_t fragSize, uint16_t fragNbLost)
{
  (void)fragCounter;
  (void)fragNb;
  (void)fragSize;
  (void)fragNbLost;
}

static void FragDecoderOnDone(int32_t status, uint32_t size)
{
  (void)status;
  (void)size;
}
