/*
 * Copyright (c) 2015-2016 Intel Corporation
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/types.h>
#include <stddef.h>
#include <string.h>
#include <errno/errno.h>
#include <sys/printk.h>
#include <sys/byteorder.h>
#include <porting.h>

#include <queue.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/services/hps.h>

#include "fsl_debug_console.h"
#include "host_msd_fatfs.h"

OSA_MSGQ_HANDLE_DEFINE(app_queue, 10, sizeof(void*));

struct bt_conn *default_conn;

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA_BYTES(BT_DATA_UUID16_ALL,
		      BT_UUID_16_ENCODE(BT_UUID_HPS_VAL))
};

static void connected(struct bt_conn *conn, uint8_t err)
{
    char addr[BT_ADDR_LE_STR_LEN];
    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    if (err)
    {
        PRINTF("Failed to connect to %s (err %u)\n", addr, err);
    }
    else
    {
		default_conn = bt_conn_ref(conn);
        PRINTF("Connected to peer: %s\n", addr);
#if CONFIG_BT_SMP
        if (bt_conn_set_security(conn, BT_SECURITY_L2))
        {
            PRINTF("Failed to set security\n");
        }
#endif
    }
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	PRINTF("Disconnected (reason 0x%02x)\n", reason);

	if (default_conn) {
		bt_conn_unref(default_conn);
		default_conn = NULL;
	}
}
#if CONFIG_BT_SMP
static void security_changed(struct bt_conn *conn, bt_security_t level, enum bt_security_err err)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Security changed: %s level %u (error %d)\n", addr, level, err);
}

static void auth_passkey_display(struct bt_conn *conn, unsigned int passkey)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Passkey for %s: %06u\n", addr, passkey);
}

static void auth_cancel(struct bt_conn *conn)
{
    char addr[BT_ADDR_LE_STR_LEN];

    bt_addr_le_to_str(bt_conn_get_dst(conn), addr, sizeof(addr));

    PRINTF("Pairing cancelled: %s\n", addr);
}
#endif
static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
#if CONFIG_BT_SMP
    .security_changed = security_changed,
#endif
};

#if CONFIG_BT_SMP
static struct bt_conn_auth_cb auth_cb_display = {
    .passkey_display = auth_passkey_display,
    .passkey_entry = NULL,
    .cancel = auth_cancel,
};
#endif
static void bt_ready(int err)
{
    if (err)
    {
        PRINTF("Bluetooth init failed (err %d)\n", err);
        return;
    }

#if (defined(CONFIG_BT_SETTINGS) && (CONFIG_BT_SETTINGS > 0))
    settings_load();
#endif /* CONFIG_BT_SETTINGS */

    PRINTF("Bluetooth initialized\n");

    bt_hps_init(app_queue);

	bt_conn_cb_register(&conn_callbacks);

#if CONFIG_BT_SMP
    bt_conn_auth_cb_register(&auth_cb_display);
#endif

    err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err)
    {
        PRINTF("Advertising failed to start (err %d)\n", err);
        return;
    }

	PRINTF("Advertising successfully started\n");
}

static void hps_process_request(void *data)
{
    hps_config_t *service_config = (hps_config_t*)data;

    PRINTF("Processing request..\r\n");

    /* Simulate a response */
    (void)service_config;

    char buf_headers[10] = {'H', 'T', 'T', 'P', 'H', 'E','A','D','E','R'};
    char buf_body[8] = {'H', 'T', 'T', 'P', 'B', 'O','D','Y'};
    write_http_headers(NULL, NULL, buf_headers, 10, 0, 0);
    write_http_entity_body(NULL, NULL, buf_body, 8, 0, 0);
    bt_hps_set_status_code(500); /* Successful */

    PRINTF("Request processed.\r\n");
}

void peripheral_hps_task(void *pvParameters)
{
    void *rx_data;
    osa_status_t ret;

    int err;

    PRINTF("BLE Peripheral HPS demo start...\n");

    ret = OSA_MsgQCreate(app_queue, 10, sizeof(void *));
    if (ret)
    {
		PRINTF("Failed to create app msg queue (err %d)\n", ret);
		return;
	}

    err = bt_enable(bt_ready);
	if (err)
    {
		PRINTF("Bluetooth init failed (err %d)\n", err);
        while (1)
        {
            vTaskDelay(2000);
        }
	}

    while(1)
    {
        ret = OSA_MsgQGet(app_queue, &rx_data, 1000);
        if (ret == KOSA_StatusSuccess)
        {
            hps_process_request(rx_data);
        }
    }
}

