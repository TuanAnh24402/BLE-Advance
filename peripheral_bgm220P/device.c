/*
 * device.c
 *
 *  Created on: Dec 25, 2024
 *      Author: AnhTT161
 */

#include "device.h"

bool led_blinking = false;
uint8_t led_state = 0;

void device_cmu_init(void)
{
  CMU_ClockEnable(cmuClock_GPIO, true);
}

void device_gpio_init(void)
{
  GPIO_PinModeSet(LED_PORT, LED_PIN, gpioModePushPull, 0);
}

void device_turn_on_led(void)
{
  GPIO_PinOutSet(LED_PORT, LED_PIN);
  led_state = 1;
}

void device_turn_off_led(void)
{
  GPIO_PinOutClear(LED_PORT, LED_PIN);
  led_state = 0;
}

void device_toggle_led(void)
{
  GPIO_PinOutToggle(LED_PORT, LED_PIN);
  led_state = 2;
}

void device_led_toggle_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  (void)handle;
  (void)data;
  if (led_blinking) {
    led_state = !led_state;
    device_toggle_led();
  }
}

void device_controll_led(uint8_t data_recv[], size_t data_recv_len)
{
  if ((data_recv_len == 8 && memcmp(data_recv, "led 1 on", 8) == 0)||
      (data_recv_len == 8 && memcmp(data_recv, "led 2 on", 8) == 0)||
      (data_recv_len == 8 && memcmp(data_recv, "led 3 on", 8) == 0)) {
    app_log("Led ON\n");
    led_blinking = false;
    device_turn_on_led();
  } else if ((data_recv_len == 9 && memcmp(data_recv, "led 1 off", 9) == 0) ||
             (data_recv_len == 9 && memcmp(data_recv, "led 2 off", 9) == 0) ||
             (data_recv_len == 9 && memcmp(data_recv, "led 3 off", 9) == 0)) {
    app_log("Led OFF\n");
    led_blinking = false;
    device_turn_off_led();
  } else if ((data_recv_len == 12 && memcmp(data_recv, "led 1 toggle", 12) == 0) ||
             (data_recv_len == 12 && memcmp(data_recv, "led 2 toggle", 12) == 0) ||
             (data_recv_len == 12 && memcmp(data_recv, "led 3 toggle", 12) == 0)) {
    led_blinking = !led_blinking;
    app_log("Led toggle\n");
  }
  else {
    app_log("Invalid attribute value\n");
  }
}
