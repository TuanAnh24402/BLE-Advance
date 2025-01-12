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
#include "mbedtls/md.h"


#ifdef USE_RANDOM_PUBLIC_ADDRESS
static void set_random_public_address(void);
#endif

// The advertising set handle allocated from Bluetooth stack.
static uint8_t advertising_set_handle = 0xff;
bd_addr address;                           // Bluetooth device address
uint8_t address_type;                      // Address type
uint8_t handle;                            // Connection handle


uint8_t adv_data[] = {
    0x02, 0x01, 0x06,
    0x08, 0x08, 'S', 'e', 'r', 'v', 'e', 'r', '1',
    0x03, 0x03, 0xFF, 0x00,
};
sl_status_t sc;

sl_sleeptimer_timer_handle_t led_timer;
extern bool led_blinking;
static uint16_t timeout = 1000;
static uint32_t timeout_ticks;
extern uint8_t led_state;
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
  uint8_t system_id[8];
  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id: {

/* Print stack version */
      app_log("Bluetooth stack booted: v%d.%d.%d-b%d\n",
                 evt->data.evt_system_boot.major,
                 evt->data.evt_system_boot.minor,
                 evt->data.evt_system_boot.patch,
                 evt->data.evt_system_boot.build);

  #ifdef USE_RANDOM_PUBLIC_ADDRESS
        set_random_public_address();
  #endif

      // Extract unique ID from BT Address.
      sc = sl_bt_system_get_identity_address(&address, &address_type);
      app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to get Bluetooth address\n",
                    (int)sc);

      /* Print Bluetooth address */
      app_log("Bluetooth %s address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                       address_type ? "static random" : "public device",
                       address.addr[5],
                       address.addr[4],
                       address.addr[3],
                       address.addr[2],
                       address.addr[1],
                       address.addr[0]);

      sc = sl_bt_sm_configure(0, sl_bt_sm_io_capability_displayonly);
      app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to configure security\n",
                    (int)sc);
      sc = sl_bt_sm_set_bondable_mode(1);
      app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to set bondalbe mode \n",
                    (int)sc);

      // Pad and reverse unique ID to get System ID.
      system_id[0] = address.addr[5];
      system_id[1] = address.addr[4];
      system_id[2] = address.addr[3];
      system_id[3] = 0xFF;
      system_id[4] = 0xFE;
      system_id[5] = address.addr[2];
      system_id[6] = address.addr[1];
      system_id[7] = address.addr[0];

      sc = sl_bt_gatt_server_write_attribute_value(gattdb_system_id,
                                                   0,
                                                   sizeof(system_id),
                                                   system_id);
      app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to write attribute\n",
                    (int)sc);

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
      break;
    }

    // -------------------------------
    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      app_log("New device connected.\n");
      handle = evt->data.evt_connection_opened.connection;

      sc = sl_bt_sm_increase_security(handle);
      app_assert(sc == SL_STATUS_OK,
                          "[E: 0x%04x] Failed to increasing security\n",
                          (int)sc);

      address = evt->data.evt_connection_opened.address;
      address_type = evt->data.evt_connection_opened.address_type;

      app_log("New connection: %d\n", handle);
      break;

    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id: {
      app_log_info("Connection closed, reason: 0x%2.2x\r\n",
                     evt->data.evt_connection_closed.reason);
      uint8_t connection = evt->data.evt_connection_closed.connection;

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
    }

    case sl_bt_evt_gatt_server_attribute_value_id: {
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
      device_controll_led(data_recv);
      break;
    }

    // -------------------------------
    // This event triggered after the pairing or bonding procedure is
    // successfully completed.
    case sl_bt_evt_sm_bonded_id:
      app_log("bond success\r\n");
      break;

    // -------------------------------
    // This event is triggered if the pairing or bonding procedure fails.
    case sl_bt_evt_sm_bonding_failed_id:
      app_log("bonding failed, reason 0x%2X\r\n",
                 evt->data.evt_sm_bonding_failed.reason);
      sc = sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);
      app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to close a Bluetooth connection\n",
                    (int)sc);
      break;

    case sl_bt_evt_gatt_server_characteristic_status_id:
      uint8_t connection = evt->data.evt_gatt_server_characteristic_status.connection;
      uint16_t characteristic = evt->data.evt_gatt_server_characteristic_status.characteristic;
//      if (evt->data.evt_gatt_server_characteristic_status.status_flags == sl_bt_gatt_server_notification) {
//          sc = sl_bt_gatt_server_send_notification(connection, characteristic, sizeof(led_state), &led_state);
//          app_log_status_error(sc);
//
//      }
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


#ifdef USE_RANDOM_PUBLIC_ADDRESS
static void set_random_public_address(void)
{
  sl_status_t sc;
  bd_addr address;
  size_t data_len;
  uint8_t data[16];

  sc = sl_bt_system_get_random_data(6, sizeof(data), &data_len, data);
  app_assert(sc == SL_STATUS_OK,
                "[E: 0x%04x] Failed to get random data\n",
                (int)sc);

  memcpy(address.addr, data, sizeof(bd_addr));
  /* set uppermost 2 bits to make this a random static address */
  address.addr[5] |= 0xC0;

  sc = sl_bt_system_set_identity_address(address, sl_bt_gap_static_address);
  app_assert(sc == SL_STATUS_OK,
                "[E: 0x%04x] Failed to set identity address\n",
                (int)sc);
}
#endif
