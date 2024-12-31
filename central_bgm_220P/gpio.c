/*
 * gpio.c
 *
 *  Created on: Dec 27, 2024
 *      Author: Anh
 */
#include "gpio.h"

void gpio_init(void)
{
  GPIO_PinModeSet(BUTTON_PORT, BUTTON_PIN, gpioModeInputPull, 1);
  GPIO_ExtIntConfig(BUTTON_PORT, BUTTON_PIN, BUTTON_PIN, false, true, true);
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

void GPIO_ODD_IRQHandler(void)
{
  uint32_t interrupt_mask = GPIO_IntGet();
  GPIO_IntClear(interrupt_mask);//Clear flag
  button_press = true;
  sl_bt_external_signal(1);
}



