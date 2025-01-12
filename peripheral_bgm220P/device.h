/*
 * device.h
 *
 *  Created on: Dec 25, 2024
 *      Author: Admin
 */
#include <stdint.h>
#include "em_gpio.h"
#include "em_cmu.h"
#include "sl_sleeptimer.h"
#include "app_log.h"
#include <stdio.h>
#include <string.h>

#ifndef DEVICE_H_
#define DEVICE_H_

#define LED_PORT              gpioPortA
#define LED_PIN               4


void device_cmu_init(void);
void device_gpio_init(void);
void device_turn_on_led(void);
void device_turn_off_led(void);
void device_toggle_led(void);

void device_led_toggle_callback(sl_sleeptimer_timer_handle_t *handle, void *data);
void device_controll_led(uint8_t data_recv[]);

#endif /* DEVICE_H_ */
