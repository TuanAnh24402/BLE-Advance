/*
 * gpio.h
 *
 *  Created on: Dec 27, 2024
 *      Author: Admin
 */
#include "em_gpio.h"
#include "stdint.h"
#include "sl_bluetooth.h"
#ifndef GPIO_H_
#define GPIO_H_

#define BUTTON_PORT                   gpioPortC
#define BUTTON_PIN                    7

bool button_press = false;
bool led_state = false;
void gpio_init(void);
void GPIO_ODD_IRQHandler(void);

#endif /* GPIO_H_ */
