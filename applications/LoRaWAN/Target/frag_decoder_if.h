/**
  ******************************************************************************
  * @file    frag_decoder_if.h
  * @brief   Application interface for the LoRaWAN fragmentation decoder.
  ******************************************************************************
  */

#ifndef FRAG_DECODER_IF_H
#define FRAG_DECODER_IF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "LmhpFragmentation.h"

#define INTEROP_TEST_MODE                            1

#define FRAG_MAX_NB                                  20
#define FRAG_MAX_SIZE                                50
#define FRAG_MAX_REDUNDANCY                          5

extern LmhpFragmentationParams_t FRAG_DECODER_IF_FragmentationParams;

#ifdef __cplusplus
}
#endif

#endif /* FRAG_DECODER_IF_H */
