/**
  ******************************************************************************
  * @file    rfm95_board.c
  * @brief   RFM95 board adapter for the SX1276 radio middleware.
  ******************************************************************************
  */

#include "rfm95_board.h"

EXTI_HandleTypeDef hRADIO_DIO_exti[RADIO_DIOn];

static void RFM95_RADIO_InitDioPin(GPIO_TypeDef *port, uint16_t pin);

int32_t RFM95_RADIO_Bus_Init(void)
{
  return RADIO_SPI_Init();
}

int32_t RFM95_RADIO_Bus_deInit(void)
{
  return RADIO_SPI_DeInit();
}

void RFM95_RADIO_IoInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  RADIO_NSS_CLK_ENABLE();
  RADIO_RESET_CLK_ENABLE();
  RADIO_DIO_0_GPIO_CLK_ENABLE();
  RADIO_DIO_1_GPIO_CLK_ENABLE();
  RADIO_DIO_2_GPIO_CLK_ENABLE();
  RADIO_DIO_3_GPIO_CLK_ENABLE();

  HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = RADIO_NSS_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RADIO_NSS_PORT, &GPIO_InitStruct);

  HAL_GPIO_WritePin(RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_PIN_SET);
  GPIO_InitStruct.Pin = RADIO_RESET_PIN;
  HAL_GPIO_Init(RADIO_RESET_PORT, &GPIO_InitStruct);

  RFM95_RADIO_InitDioPin(RADIO_DIO_0_PORT, RADIO_DIO_0_PIN);
  RFM95_RADIO_InitDioPin(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN);
  RFM95_RADIO_InitDioPin(RADIO_DIO_2_PORT, RADIO_DIO_2_PIN);
  RFM95_RADIO_InitDioPin(RADIO_DIO_3_PORT, RADIO_DIO_3_PIN);
}

void RFM95_RADIO_IoDeInit(void)
{
  HAL_GPIO_DeInit(RADIO_NSS_PORT, RADIO_NSS_PIN);
  HAL_GPIO_DeInit(RADIO_RESET_PORT, RADIO_RESET_PIN);
  HAL_GPIO_DeInit(RADIO_DIO_0_PORT, RADIO_DIO_0_PIN);
  HAL_GPIO_DeInit(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN);
  HAL_GPIO_DeInit(RADIO_DIO_2_PORT, RADIO_DIO_2_PIN);
  HAL_GPIO_DeInit(RADIO_DIO_3_PORT, RADIO_DIO_3_PIN);
}

void RFM95_RADIO_IoIrqInit(DioIrqHandler **irqHandlers)
{
  HAL_EXTI_GetHandle(&hRADIO_DIO_exti[0], RADIO_DIO_0_EXTI_LINE);
  HAL_EXTI_GetHandle(&hRADIO_DIO_exti[1], RADIO_DIO_1_EXTI_LINE);
  HAL_EXTI_GetHandle(&hRADIO_DIO_exti[2], RADIO_DIO_2_EXTI_LINE);
  HAL_EXTI_GetHandle(&hRADIO_DIO_exti[3], RADIO_DIO_3_EXTI_LINE);

  HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[0], HAL_EXTI_COMMON_CB_ID, irqHandlers[0]);
  HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[1], HAL_EXTI_COMMON_CB_ID, irqHandlers[1]);
  HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[2], HAL_EXTI_COMMON_CB_ID, irqHandlers[2]);
  HAL_EXTI_RegisterCallback(&hRADIO_DIO_exti[3], HAL_EXTI_COMMON_CB_ID, irqHandlers[3]);

  HAL_NVIC_SetPriority(RADIO_DIO_0_IRQn, RADIO_DIO_0_IT_PRIO, 0);
  HAL_NVIC_EnableIRQ(RADIO_DIO_0_IRQn);
  HAL_NVIC_SetPriority(RADIO_DIO_1_IRQn, RADIO_DIO_1_IT_PRIO, 0);
  HAL_NVIC_EnableIRQ(RADIO_DIO_1_IRQn);
  HAL_NVIC_SetPriority(RADIO_DIO_2_IRQn, RADIO_DIO_2_IT_PRIO, 0);
  HAL_NVIC_EnableIRQ(RADIO_DIO_2_IRQn);
  HAL_NVIC_SetPriority(RADIO_DIO_3_IRQn, RADIO_DIO_3_IT_PRIO, 0);
  HAL_NVIC_EnableIRQ(RADIO_DIO_3_IRQn);
}

uint8_t RFM95_RADIO_SendRecv(uint8_t txData)
{
  uint8_t rxData = 0;

  if (RADIO_SPI_SendRecv(&txData, &rxData, 1) != 1)
  {
    return 0;
  }

  return rxData;
}

void RFM95_RADIO_ChipSelect(uint8_t state)
{
  HAL_GPIO_WritePin(RADIO_NSS_PORT, RADIO_NSS_PIN, state == 0U ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

bool RFM95_RADIO_CheckRfFrequency(uint32_t frequency)
{
  (void)frequency;
  return true;
}

void RFM95_RADIO_Reset(void)
{
  HAL_GPIO_WritePin(RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(2);
  HAL_GPIO_WritePin(RADIO_RESET_PORT, RADIO_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(6);
}

void RFM95_RADIO_SetXO(uint8_t state)
{
  (void)state;
}

uint32_t RFM95_RADIO_GetWakeUpTime(void)
{
  return 2U;
}

uint8_t RFM95_RADIO_GetPaSelect(uint32_t channel)
{
  (void)channel;
  return CONF_RFO_HP;
}

void RFM95_RADIO_SetAntSw(uint8_t state)
{
  (void)state;
}

uint32_t RFM95_RADIO_GetDio1PinState(void)
{
  return HAL_GPIO_ReadPin(RADIO_DIO_1_PORT, RADIO_DIO_1_PIN) == GPIO_PIN_SET ? 1U : 0U;
}

static void RFM95_RADIO_InitDioPin(GPIO_TypeDef *port, uint16_t pin)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(port, &GPIO_InitStruct);
}
