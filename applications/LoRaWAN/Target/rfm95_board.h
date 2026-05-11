/**
  ******************************************************************************
  * @file    rfm95_board.h
  * @brief   RFM95 board adapter for the SX1276 radio middleware.
  ******************************************************************************
  */

#ifndef RFM95_BOARD_H
#define RFM95_BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include "nucleo_l476rg_bus.h"
#include "platform.h"
#include "stm32l4xx_hal.h"

typedef void(DioIrqHandler)(void);

#define RADIO_DIO_0_IT_PRIO                        0
#define RADIO_DIO_1_IT_PRIO                        0
#define RADIO_DIO_2_IT_PRIO                        0
#define RADIO_DIO_3_IT_PRIO                        0

#define RADIO_SPI_Init                   BSP_SPI1_Init
#define RADIO_SPI_DeInit                 BSP_SPI1_DeInit
#define RADIO_SPI_SendRecv               BSP_SPI1_SendRecv

#define RADIO_NSS_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_NSS_PORT                   GPIOB
#define RADIO_NSS_PIN                    GPIO_PIN_6

#define RADIO_RESET_CLK_ENABLE()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_RESET_PORT                 GPIOA
#define RADIO_RESET_PIN                  GPIO_PIN_4

#define RADIO_DIOn                       4U

#define RADIO_DIO_0_PORT                 GPIOA
#define RADIO_DIO_0_PIN                  GPIO_PIN_10
#define RADIO_DIO_0_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define RADIO_DIO_0_EXTI_LINE            EXTI_LINE_10
#define RADIO_DIO_0_IRQn                 EXTI15_10_IRQn
#define H_EXTI_10                        hRADIO_DIO_exti[0]

#define RADIO_DIO_1_PORT                 GPIOB
#define RADIO_DIO_1_PIN                  GPIO_PIN_3
#define RADIO_DIO_1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_1_EXTI_LINE            EXTI_LINE_3
#define RADIO_DIO_1_IRQn                 EXTI3_IRQn
#define H_EXTI_3                         hRADIO_DIO_exti[1]

#define RADIO_DIO_2_PORT                 GPIOB
#define RADIO_DIO_2_PIN                  GPIO_PIN_5
#define RADIO_DIO_2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_2_EXTI_LINE            EXTI_LINE_5
#define RADIO_DIO_2_IRQn                 EXTI9_5_IRQn
#define H_EXTI_5                         hRADIO_DIO_exti[2]

#define RADIO_DIO_3_PORT                 GPIOB
#define RADIO_DIO_3_PIN                  GPIO_PIN_4
#define RADIO_DIO_3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define RADIO_DIO_3_EXTI_LINE            EXTI_LINE_4
#define RADIO_DIO_3_IRQn                 EXTI4_IRQn
#define H_EXTI_4                         hRADIO_DIO_exti[3]

#define RFSW_OFF                         0U
#define RFSW_RX                          1U
#define RFSW_RFO_LP                      2U
#define RFSW_RFO_HP                      3U

#define CONF_RFO_LP                      0U
#define CONF_RFO_HP                      1U
#define CONF_RFO_LP_HP                   2U
#define CONF_RFO_LF                      3U

extern EXTI_HandleTypeDef hRADIO_DIO_exti[RADIO_DIOn];

int32_t RFM95_RADIO_Bus_Init(void);
int32_t RFM95_RADIO_Bus_deInit(void);
void RFM95_RADIO_IoInit(void);
void RFM95_RADIO_IoDeInit(void);
void RFM95_RADIO_IoIrqInit(DioIrqHandler **irqHandlers);
uint8_t RFM95_RADIO_SendRecv(uint8_t txData);
void RFM95_RADIO_ChipSelect(uint8_t state);
bool RFM95_RADIO_CheckRfFrequency(uint32_t frequency);
void RFM95_RADIO_Reset(void);
void RFM95_RADIO_SetXO(uint8_t state);
uint32_t RFM95_RADIO_GetWakeUpTime(void);
uint8_t RFM95_RADIO_GetPaSelect(uint32_t channel);
void RFM95_RADIO_SetAntSw(uint8_t state);
uint32_t RFM95_RADIO_GetDio1PinState(void);

#ifdef __cplusplus
}
#endif

#endif /* RFM95_BOARD_H */
