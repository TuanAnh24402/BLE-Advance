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

void device_controll_led(uint8_t data_recv[])
{
  if (strstr((const char*)data_recv, "on") != NULL) {
    app_log("Led ON\n");
    led_blinking = false;
    device_turn_on_led();
  } else if (strstr((const char*)data_recv, "off") != NULL) {
    app_log("Led OFF\n");
    led_blinking = false;
    device_turn_off_led();
  } else if (strstr((const char*)data_recv, "toggle") != NULL) {
    led_blinking = !led_blinking;
    app_log("Led toggle\n");
  }
  else {
    app_log("Invalid attribute value\n");
  }
}
