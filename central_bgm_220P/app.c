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
#include "mbedtls/md.h"
#include <stdio.h>
#include <string.h>
#include "gatt_db.h"

// The advertising set handle allocated from Bluetooth stack.
// Handle for the advertising set
static uint8_t advertising_set_handle = 0xff;
bd_addr address;                           // Bluetooth device address
uint8_t address_type;                      // Address type
uint8_t handle;                            // Connection handle

static conn_state_t state;
static sl_status_t sc;
static uint8_t live_connections = 0;
static connection_info_t conn[MAX_CONNECTION];
// Array for holding properties of multiple (parallel) connections

//Target service UUID
static const uint8_t service_uuid[2] = {0xFF, 0x00};

//BLE Handle
static uint32_t service_handle = 0;
static uint16_t characteristic_handle[2];
bool check_characteristic = true;
static uint8_t smartphone_connection = 0;

static uint8_t led_state_1 = 0;
static uint8_t led_state_2 = 0;
static uint8_t led_state_3 = 0;

static char *led_handle[MAX_CONNECTION] = {"led 1", "led 2", "led 3"};

static void init_properties(void);
static void print_bluetooth_address(void);
static bd_addr *read_and_cache_bluetooth_address(uint8_t *address_type_out);
static uint8_t find_service_in_advertisement(uint8_t *data, uint8_t len);
static uint8_t find_index_by_connection_handle(uint8_t connection);
static void add_connection(uint8_t connection, uint16_t address);
// Remove a connection from the connection_properties array
static void remove_connection(uint8_t connection);
void sl_update_advertising_data();
void sl_start_advertising();
void sl_recieved_data(uint8_t connection, uint16_t characteristic, uint8array *received_value);
void sl_read_data(uint8_t connection);
void sl_controll_led(uint8_t data_recv[], size_t data_recv_len);
/**************************************************************************//**
 * Application Init.
 *****************************************************************************/
SL_WEAK void app_init(void)
{
  init_properties();
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
  uint32_t passkey_central = 0;
  uint8_t table_index;
  uint16_t addr_value;

  switch (SL_BT_MSG_ID(evt->header)) {
    // -------------------------------
    // This event indicates the device has started and the radio is ready.
    // Do not call any stack command before receiving this boot event!
    case sl_bt_evt_system_boot_id: {
      sl_start_advertising();

      char output[100];
      sprintf(output, "Bluetooth stack booted: v%d.%d.%d-b%d\n",
              evt->data.evt_system_boot.major,
              evt->data.evt_system_boot.minor,
              evt->data.evt_system_boot.patch,
              evt->data.evt_system_boot.build);
      app_log(output);
      print_bluetooth_address();
      uint16_t scan_interval = 160;
      uint16_t scan_window = 160;

      // Set scan parameters
      sc = sl_bt_scanner_set_parameters(sl_bt_scanner_scan_mode_passive,  // passive scan
                                        scan_interval,
                                        scan_window);
      app_assert_status(sc);

      // Set the default connection parameters for subsequent connections
       sc = sl_bt_connection_set_default_parameters(CONN_INTERVAL_MIN,
                                                    CONN_INTERVAL_MAX,
                                                    CONN_RESPONDER_LATENCY,
                                                    CONN_TIMEOUT,
                                                    CONN_MIN_CE_LENGTH,
                                                    CONN_MAX_CE_LENGTH);
      app_assert_status(sc);
      // Start scanning - looking for devices
      sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                                sl_bt_scanner_discover_generic);
      app_assert_status(sc);
      state = scanning;

      passkey_central = 123456;
      sc = sl_bt_sm_configure(0x00, sl_bt_sm_io_capability_displayonly);
      app_assert(sc == SL_STATUS_OK,
                 "[E: 0x%04x] Failed to configure security\n",
                 (int)sc);

      sc = sl_bt_sm_set_passkey(passkey_central);
      app_assert(sc == SL_STATUS_OK,
                   "[E: 0x%04x] Failed to set passkey\n",
                   (int)sc);

      sc = sl_bt_sm_set_bondable_mode(1);
      app_assert(sc == SL_STATUS_OK,
                 "[E: 0x%04x] Failed to set bondalbe mode \n",
                 (int)sc);

      break;
    }

    case sl_bt_evt_scanner_legacy_advertisement_report_id:
      uint8_t *adv_data = evt->data.evt_scanner_legacy_advertisement_report.data.data;
      uint8_t adv_len = evt->data.evt_scanner_legacy_advertisement_report.data.len;
      uint8_t event_flags = evt->data.evt_scanner_legacy_advertisement_report.event_flags;
      if (event_flags == (SL_BT_SCANNER_EVENT_FLAG_CONNECTABLE | SL_BT_SCANNER_EVENT_FLAG_SCANNABLE)) {
       // If a thermometer advertisement is found...
        if (find_service_in_advertisement(adv_data, adv_len) != 0) {
           // then stop scanning for a while
           sc = sl_bt_scanner_stop();
           app_assert_status(sc);
           // and connect to that device
           if (live_connections < MAX_CONNECTION) {
             sc = sl_bt_connection_open(evt->data.evt_scanner_legacy_advertisement_report.address,
                                        evt->data.evt_scanner_legacy_advertisement_report.address_type,
                                        sl_bt_gap_phy_1m,
                                        NULL);

             app_assert_status(sc);
             state = opening;
           }
         }
       }
      break;

    // This event indicates that a new connection was opened.
    case sl_bt_evt_connection_opened_id:
      uint8_t current_connection = evt->data.evt_connection_opened.connection;
      bd_addr add_current_connection = evt->data.evt_connection_opened.address;
      uint8_t type_add_connection = evt->data.evt_connection_opened.address_type;
      app_log("Type address connection: %d\n", type_add_connection);

      app_log("add current connection: %02X:%02X:%02X:%02X:%02X:%02X\n",
              add_current_connection.addr[5],
              add_current_connection.addr[4],
              add_current_connection.addr[3],
              add_current_connection.addr[2],
              add_current_connection.addr[1],
              add_current_connection.addr[0]);

      if (type_add_connection != sl_bt_gap_random_resolvable_address) {
        app_log("Server connected\n");
        sc = sl_bt_gatt_discover_primary_services_by_uuid(current_connection,
                                                          sizeof(service_uuid),
                                                          (const uint8_t*) service_uuid);

         if (sc == SL_STATUS_INVALID_HANDLE) {
           // Failed to open connection, res-tart scanning
           app_log_warning("Primary service discovery failed with invalid handle, dropping client\n");
           sc = sl_bt_scanner_start(sl_bt_gap_phy_1m, sl_bt_scanner_discover_generic);
           app_assert_status(sc);
           state = scanning;
           sc = sl_bt_connection_close(current_connection);
           app_assert_status(sc);
           break;
         }
         else {
           app_assert_status(sc);
         }
         // Get last two bytes of sender address
         addr_value = (uint16_t)(evt->data.evt_connection_opened.address.addr[1] << 8) + evt->data.evt_connection_opened.address.addr[0];
         // Add connection to the connection_properties array
         add_connection(evt->data.evt_connection_opened.connection, addr_value);
         state = discover_services;
      } else {
        sc = sl_bt_sm_increase_security(current_connection);
        app_assert(sc == SL_STATUS_OK,
                  "[E: 0x%04x] Failed to increasing security\n",
                  (int)sc);
        app_log("Smartphone is connected\n");
        smartphone_connection = current_connection;
        sl_bt_advertiser_stop(advertising_set_handle);
      }
      break;
    // -------------------------------
    // This event is generated when a new service is discovered
    case sl_bt_evt_gatt_service_id:
      service_handle = evt->data.evt_gatt_service.service;
      const uint8_t *uuid = evt->data.evt_gatt_service.uuid.data;
      size_t uuid_len = evt->data.evt_gatt_service.uuid.len;
      uint8_t connection = evt->data.evt_gatt_service.connection;
      table_index = find_index_by_connection_handle(connection);
      if (table_index != TABLE_INDEX_INVALID) {
        // Save service handle for future reference
        conn[table_index].led_service_handle = service_handle;
      }

      app_log("Service found with UUID: ");
      for (size_t i = 0; i < uuid_len; i++) {
        char output[20];
        sprintf(output, "%02X", uuid[i]);
        app_log(output);
        if (i < uuid_len - 1) {
          app_log(":");
        }
      }
      app_log("\n");
      app_log("Discovering characteristics start ...\n");
      break;

    // -------------------------------
    // This event is generated when a new characteristic is discovered
    case sl_bt_evt_gatt_characteristic_id: {
      uint16_t handle = evt->data.evt_gatt_characteristic.characteristic;
      const uint8_t *uuid_char = evt->data.evt_gatt_characteristic.uuid.data;
      size_t uuid_char_len = evt->data.evt_gatt_characteristic.uuid.len;
      table_index = find_index_by_connection_handle(evt->data.evt_gatt_characteristic.connection);
      if (table_index != TABLE_INDEX_INVALID) {
        // Save characteristic handle for future reference
        conn[table_index].led_characteristic_handle = handle;
      }

      if (uuid_char_len == UUID_CHARACTERISTIC_LENGHT) {
        uint16_t uuid16 = uuid_char[1] << 8 | uuid_char[0];
        int index = -1;
        switch (uuid16) {
          case LED_CONTROL_UUID:
            index = LED_CONTROL;
            break;
        }

        if (index != -1) {
          characteristic_handle[index] =  handle;
          char output[50];
          sprintf(output, "Saved characteristic with UUID 0x%04x at index %d\n", uuid16, index);
          app_log(output);
          app_log("discover characteristics finished\n");
          break;
        }
      }
      break;
    }

    case sl_bt_evt_gatt_characteristic_value_id: {
      uint8_t connection = evt->data.evt_gatt_characteristic_value.connection;
      uint16_t characteristic = evt->data.evt_gatt_characteristic_value.characteristic;

      uint8_t *value = evt->data.evt_gatt_characteristic_value.value.data;
      table_index = find_index_by_connection_handle(connection);
      if (table_index == TABLE_INDEX_INVALID) {
        break;
      }
      if (characteristic == gattdb_led) {
        uint8_t led_state = value[0];
        app_log("led state: %d\n",led_state);
      }
      break;
    }
    case sl_bt_evt_gatt_server_attribute_value_id: {
      uint16_t attribute = evt->data.evt_gatt_server_attribute_value.attribute;
      uint8_t data_recv[50];
      size_t data_recv_len;

      if (attribute == gattdb_led) {
        sc = sl_bt_gatt_server_read_attribute_value(attribute,
                                                    0,
                                                    sizeof(data_recv),
                                                    &data_recv_len,
                                                    data_recv);
      }
      (void)data_recv_len;
      app_log_status_error(sc);
      sl_controll_led(data_recv, data_recv_len);

      if (sc != SL_STATUS_OK) {
        break;
      }
//      state = discover_characteristics;
      break;
    }

    // -------------------------------
    // This event is generated for various procedure completions, e.g. when a
    // write procedure is completed, or service discovery is completed
    case sl_bt_evt_gatt_procedure_completed_id: {
      uint8_t connection = evt->data.evt_gatt_procedure_completed.connection;
      table_index = find_index_by_connection_handle(connection);
      app_log("state = %d\n",state);

      // If service discovery finished
      if (state == discover_services && conn[table_index].led_service_handle != SERVICE_HANDLE_INVALID) {
        // Discover thermometer characteristic on the responder device
        sc = sl_bt_gatt_discover_characteristics(connection,
                                                 conn[table_index].led_service_handle);
        app_assert_status(sc);
        state = discover_characteristics;
        break;
      }
      if (state == discover_characteristics && conn[table_index].led_characteristic_handle != CHARACTERISTIC_HANDLE_INVALID) {
        // stop discovering
        sc = sl_bt_scanner_stop();
        app_assert_status(sc);

        // enable notification
        sc = sl_bt_gatt_set_characteristic_notification(evt->data.evt_gatt_procedure_completed.connection,
                                                        conn[table_index].led_characteristic_handle,
                                                        sl_bt_gatt_notification);
        app_assert_status(sc);
        app_log("notify enable\n");
        state = notification;
      }
      // If indication enable process finished
      if (state == notification) {
         app_log("state = %d\n",state);

        // and we can connect to more devices
        if (live_connections < MAX_CONNECTION) {
          // start scanning again to find new devices
          sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                                   sl_bt_scanner_discover_generic);
          app_assert_status_f(sc, "Failed to start discovery #2" APP_LOG_NL);
          state = scanning;
        } else {
          state = running;
        }
        break;
      }
      break;
    }
    case sl_bt_evt_sm_confirm_bonding_id:{
      app_log_info("Bonding confirm\r\n");
      uint8_t connection = evt->data.evt_sm_confirm_bonding.connection;
      table_index = find_index_by_connection_handle(connection);

      if (connection == conn[table_index].handle) {
        sc = sl_bt_sm_bonding_confirm(connection, 1);
        app_assert_status(sc);
        app_log_info("Bonding %d confirm\r\n", table_index);
      }
      break;
    }

    case sl_bt_evt_sm_passkey_request_id: {
      app_log("Passkey request\r\n");
      uint8_t connection = evt->data.evt_sm_passkey_request.connection;
      uint32_t passkey = 123456;

      if (connection == conn[0].handle) {
          app_log("Enter paskey 1\n");
          sl_status_t sc = sl_bt_sm_enter_passkey(connection, passkey);
          app_assert(sc == SL_STATUS_OK,
                     "[E: 0x%04x] Failed to enter passkey\n",
                     (int)sc);
      } else if (connection == conn[1].handle) {
          app_log("Enter paskey 2\n");
        sl_status_t sc = sl_bt_sm_enter_passkey(connection, passkey);
                  app_assert(sc == SL_STATUS_OK,
                             "[E: 0x%04x] Failed to enter passkey\n",
                             (int)sc);
      } else if (connection == conn[2].handle) {
          app_log("Enter paskey 3\n");
        sl_status_t sc = sl_bt_sm_enter_passkey(connection, passkey);
         app_assert(sc == SL_STATUS_OK,
                    "[E: 0x%04x] Failed to enter passkey\n",
                    (int)sc);
      }
      break;
    }
    // -------------------------------
    // This event indicates a request to display the passkey to the user.
    case sl_bt_evt_sm_passkey_display_id:
      app_log("passkey: %4ld\r\n", evt->data.evt_sm_passkey_display.passkey);
      break;

    case sl_bt_evt_sm_bonded_id: {
      uint8_t connection = evt->data.evt_sm_bonded.connection;
      table_index = find_index_by_connection_handle(connection);

      if (connection == conn[table_index].handle) {
          app_log("Bonding %d successful \r\n", table_index);
      }
      if (connection == smartphone_connection) {
          app_log("bond smartphone success\r\n");
      }
      break;

    }

    case sl_bt_evt_sm_bonding_failed_id: {
      app_log("Bonding failed, reason 0x%2X\r\n",
              evt->data.evt_sm_bonding_failed.reason);

      sc = sl_bt_connection_close(evt->data.evt_sm_bonding_failed.connection);
      app_assert(sc == SL_STATUS_OK,
                 "[E: 0x%04x] Failed to close Bluetooth connection\n",
                 (int)sc);
      break;
    }
    // -------------------------------
    // This event indicates that a connection was closed.
    case sl_bt_evt_connection_closed_id: {
      // remove connection from active connections
      uint8_t connection = evt->data.evt_connection_closed.connection;
      if (connection == smartphone_connection) {
          app_log("Smart phone disconnected\n");
          // Restart advertising after client has disconnected.
          sc = sl_bt_legacy_advertiser_start(advertising_set_handle,
                                             sl_bt_legacy_advertiser_connectable);
          app_log("Start advertising\n");

          app_assert_status(sc);
          break;
      }
      else {
        remove_connection(connection);
        if (state != scanning) {
          // start scanning again to find new devices
          sc = sl_bt_scanner_start(sl_bt_scanner_scan_phy_1m,
                                   sl_bt_scanner_discover_generic);
          app_assert_status_f(sc, "Failed to start discovery #3" APP_LOG_NL);
          state = scanning;
        }
      }

      break;
    }

    ///////////////////////////////////////////////////////////////////////////
    // Add additional event handlers here as your application requires!      //
    ///////////////////////////////////////////////////////////////////////////

    // -------------------------------
    // Default event handler.
    default:
      break;
  }
}

/**************************************************************************//**
 * @brief
 *   Function to Read and Cache Bluetooth Address.
 * @param address_type_out [out]
 *   A pointer to the outgoing address_type. This pointer can be NULL.
 * @return
 *   Pointer to the cached Bluetooth Address
 *****************************************************************************/
static bd_addr *read_and_cache_bluetooth_address(uint8_t *address_type_out)
{
  static bd_addr address;
  static uint8_t address_type;
  static bool cached = false;

  if (!cached) {
    sl_status_t sc = sl_bt_system_get_identity_address(&address, &address_type);
    app_assert_status(sc);
    cached = true;
  }

  if (address_type_out) {
    *address_type_out = address_type;
  }
  return &address;
}

static void print_bluetooth_address(void)
{
  uint8_t address_type;
  bd_addr *address = read_and_cache_bluetooth_address(&address_type);

  char output[100];
  sprintf(output, "Bluetooth %s address: %02X:%02X:%02X:%02X:%02X:%02X\n",
          address_type ? "static random" : "public device",
          address->addr[5],
          address->addr[4],
          address->addr[3],
          address->addr[2],
          address->addr[1],
          address->addr[0]);
  app_log(output);
}

void sl_update_advertising_data() {

    uint8_t adv_data[] = {
        0x02, 0x01, 0x06,                      // Flags: 0x06 (General Discoverable Mode, BR/EDR Not Supported)
        0x07, 0x09, 'S','i','l','l','a','b',
        0x03, 0xFF, 0x02, 0x04                 // Length and Manufacturer Specific Data Type
    };
    sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, sizeof(adv_data), adv_data);
    app_assert_status(sc);

    // Set advertising parameters. Advertising interval is set to 100 ms.
    sc = sl_bt_advertiser_set_timing(
        advertising_set_handle,
        160, // min. adv. interval (milliseconds * 1.6)
        160, // max. adv. interval (milliseconds * 1.6)
        0,   // adv. duration
        0);  // max. num. adv. events
    app_assert_status(sc);

    sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);
    app_assert_status(sc);
}

// Start advertises
void sl_start_advertising() {
    // Create an advertising set
    sc = sl_bt_advertiser_create_set(&advertising_set_handle);
    app_assert_status(sc);

    // Update data advertises
    sl_update_advertising_data();
}

void sl_read_data(uint8_t connection) {
  if (connection == conn[0].handle) {
    sc = sl_bt_gatt_read_characteristic_value(conn[0].handle,characteristic_handle[0]);
    app_log("read1\n");
  }
  else if (connection == conn[1].handle) {
    sc = sl_bt_gatt_read_characteristic_value(conn[1].handle,characteristic_handle[0]);
    app_log("read2\n");

  }
  else if (connection == conn[2].handle) {
    sc = sl_bt_gatt_read_characteristic_value(conn[2].handle,characteristic_handle[0]);
    app_log("read3\n");

  }

}

void sl_recieved_data(uint8_t connection, uint16_t characteristic, uint8array *received_value) {
  if (connection == conn[0].handle && characteristic == characteristic_handle[0]) {
      app_log("LED state received value by server 1: %d\n", received_value->data[0]);
      led_state_1 = received_value->data[0];
  } else if (connection == conn[1].handle && characteristic == characteristic_handle[0]) {
      app_log("LED state received value by server 2: %d\n", received_value->data[0]);
      led_state_2 = received_value->data[0];
  } else if (connection == conn[2].handle && characteristic == characteristic_handle[0]) {
      app_log("LED state received value by server 3: %d\n", received_value->data[0]);
      led_state_3 = received_value->data[0];
  } else {
      app_log("Received unknown characteristic value\n");
  }
}

void sl_controll_led(uint8_t data_recv[], size_t data_recv_len)
{


  for (int i = 0 ;i < MAX_CONNECTION; i++) {
    if (memcmp(data_recv, led_handle[i], 5) == 0) {
      sc = sl_bt_gatt_write_characteristic_value(conn[i].handle,
                                                 characteristic_handle[0],
                                                 data_recv_len,
                                                 data_recv);
      app_assert_status(sc);
    }
  }
}

// Parse advertisements looking for advertised Health Thermometer service
static uint8_t find_service_in_advertisement(uint8_t *data, uint8_t len)
{
  uint8_t ad_field_length;
  uint8_t ad_field_type;
  uint8_t i = 0;
  // Parse advertisement packet
  while (i < len) {
    ad_field_length = data[i];
    ad_field_type = data[i + 1];
    // Partial ($02) or complete ($03) list of 16-bit UUIDs
    if (ad_field_type == 0x02 || ad_field_type == 0x03) {
      // compare UUID to service UUID
      if (memcmp(&data[i + 2], service_uuid, 2) == 0) {
        return 1;
      }
    }
    // advance to the next AD struct
    i = i + ad_field_length + 1;
  }
  return 0;
}

// Find the index of a given connection in the connection_properties array
static uint8_t find_index_by_connection_handle(uint8_t connection)
{
  for (uint8_t i = 0; i < live_connections; i++) {
    if (conn[i].handle == connection) {
      return i;
    }
  }
  return TABLE_INDEX_INVALID;
}

// Add a new connection to the connection_properties array
static void add_connection(uint8_t connection, uint16_t address)
{
  conn[live_connections].handle = connection;
  conn[live_connections].server_address = address;
  live_connections++;
  app_log("live connections: %d\n", live_connections);
}

// Remove a connection from the connection_properties array
static void remove_connection(uint8_t connection)
{
  uint8_t i;
  uint8_t table_index = find_index_by_connection_handle(connection);

  if (live_connections > 0) {
    live_connections--;
  }
  // Shift entries after the removed connection toward 0 index
  for (i = table_index; i < live_connections; i++) {
      conn[i] = conn[i + 1];
  }
  // Clear the slots we've just removed so no junk values appear
  for (i = live_connections; i < MAX_CONNECTION; i++) {
      conn[i].handle = CONNECTION_HANDLE_INVALID;

  }
}

// Init connection properties
static void init_properties(void)
{
  uint8_t i;
  live_connections = 0;

  for (i = 0; i < MAX_CONNECTION; i++) {
    conn[i].handle = CONNECTION_HANDLE_INVALID;
    conn[i].led_service_handle = SERVICE_HANDLE_INVALID;
    conn[i].led_characteristic_handle = CHARACTERISTIC_HANDLE_INVALID;
  }
}
