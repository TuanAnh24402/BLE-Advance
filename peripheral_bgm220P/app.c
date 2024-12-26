/***************************************************************************//**
 * @file
 * @brief Core application logic.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "app.h"
#include "device.h"
#include "gatt_db.h"
#include <stdio.h>
#include <string.h>

#define MAX_CONNECTION              1
// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
bd_addr address;                           // Bluetooth device address
uint8_t address_type;                      // Address type
uint8_t handle;                            // Connection handle
static uint8_t current_connections = 0;


uint8_t adv_data[] = {
    0x02, 0x01, 0x06,
    0x08, 0x08, 'S', 'e', 'r', 'v', 'e', 'r', '2',
    0x03, 0x03, 0xFF, 0x00,
};
sl_status_t sc;

sl_sleeptimer_timer_handle_t led_timer;
extern bool led_blinking;
static uint16_t timeout = 1000;
static uint32_t timeout_ticks;
/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  device_cmu_init();
  device_gpio_init();
  timeout_ticks = sl_sleeptimer_ms_to_tick(timeout);
  sl_sleeptimer_start_periodic_timer(&led_timer, timeout_ticks, device_led_toggle_callback, NULL, 0, 0);
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application init code here!                         //
  // This is called once during start-up.                                    //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Application Process Action.
 *****************************************************************************/
SL_WEAK void app_process_action(void)
{
  /////////////////////////////////////////////////////////////////////////////
  // Put your additional application code here!                              //
  // This is called infinitely.                                              //
  // Do not call blocking functions from here!                               //
  /////////////////////////////////////////////////////////////////////////////
}

/**************************************************************************//**
 * Bluetooth stack event handler.
 * This overrides the dummy weak implementation.
 *
 * @param[in] evt Event coming from the Bluetooth stack.
 *****************************************************************************/
void sl_bt_on_event(sl_bt_msg_t *evt)
{
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id:
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&advertising_set_handle);
      app_assert_status(sc);

      sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle,
                                                  0,
                                                  sizeof(adv_data),
                                                  adv_data);
      app_assert_status(sc);

      // Set advertising interval to 100ms.
      sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
      app_assert_status(sc);

      // Start advertising and enable connections.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                               sl_bt_legacy_advertiser_connectable);
      app_log("Start advertising\n");
      app_assert_status(sc);
      if (current_connections < MAX_CONNECTION) {
        // Tiếp tục quảng bá nếu chưa đạt số kết nối tối đa
        sl_bt_legacy_advertiser_start(advertising_set_handle,
                                      sl_bt_legacy_advertiser_connectable);
      } else {
        // Dừng quảng bá nếu đã đạt số kết nối tối đa
        sl_bt_advertiser_stop(advertising_set_handle);
      }

      break;

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      app_log("New device connected.\n");
      current_connections++;

      address = evt->data.evt_connection_opened.address;
      address_type = evt->data.evt_connection_opened.address_type;
      handle = evt->data.evt_connection_opened.connection;
      if (current_connections < MAX_CONNECTION) {
        // Tiếp tục quảng bá nếu chưa đạt số kết nối tối đa
        sl_bt_legacy_advertiser_start(advertising_set_handle,
                                      sl_bt_legacy_advertiser_connectable);
      } else {
        // Dừng quảng bá nếu đã đạt số kết nối tối đa
        sl_bt_advertiser_stop(advertising_set_handle);
      }

      app_log("New connection: %d\n", handle);
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id:
      uint8_t connection = evt->data.evt_connection_closed.connection;
      current_connections--;

      // Generate data for advertising
      app_log( "Device has connection %d disconnected\n", connection);

      sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle,
                                            0,
                                            sizeof(adv_data),
                                            adv_data);
      app_assert_status(sc);

      // Restart advertising after client has disconnected.
      sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                         sl_bt_legacy_advertiser_connectable);
      app_log("Start advertising\n");

      app_assert_status(sc);
      break;

    case sl_bt_evt_gatt_server_attribute_value_id:
      uint16_t attribute = evt->data.evt_gatt_server_attribute_value.attribute;
      uint8_t data_recv[50];
      size_t data_recv_len;

      if (attribute == gattdb_led_control) {
        sc = sl_bt_gatt_server_read_attribute_value(attribute,
                                                    0,
                                                    sizeof(data_recv),
                                                    &data_recv_len,
                                                    data_recv);
      }

      (void)data_recv_len;
      app_log_status_error(sc);

      if (sc != SL_STATUS_OK) {
        break;
      }

      if (data_recv_len == 6 && memcmp(data_recv, "led on", 6) == 0) {
        app_log("Led ON\n");
        led_blinking = false;
        device_turn_on_led();
      }
      else if (data_recv_len == 7 && memcmp(data_recv, "led off", 7) == 0) {
        app_log("Led OFF\n");
        led_blinking = false;
        device_turn_off_led();
      }
      else if (data_recv_len == 10 && memcmp(data_recv, "led toggle", 10) == 0) {
        led_blinking = !led_blinking;
        app_log("Led toggle\n");
      }
      else {
        app_log("Invalid attribute value\n");
      }
      break;

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}
