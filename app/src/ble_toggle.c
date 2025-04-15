#include <zephyr/device.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <zmk/behavior.h>
#include <zmk/endpoints.h>
#include <zmk/ble.h>
#include <zmk/usb.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static bool ble_on = true;

int ble_toggle_behavior(struct zmk_behavior_binding *binding, struct zmk_behavior_binding_event event) {
    if (zmk_usb_is_active()) {
        LOG_INF("USB is active — BLE will stay OFF.");
        zmk_ble_advertise_off();
        ble_on = false;
    } else {
        if (ble_on) {
            LOG_INF("Disabling BLE advertising (manual toggle)");
            zmk_ble_advertise_off();
        } else {
            LOG_INF("Enabling BLE advertising (manual toggle)");
            zmk_ble_advertise_on();
        }
        ble_on = !ble_on;
    }
    return 0;
}

static const struct behavior_driver_api ble_toggle_driver_api = {
    .binding_pressed = ble_toggle_behavior,
};

DEVICE_DT_DEFINE(DT_INST(0, zmk_behavior_ble_toggle), NULL, NULL, NULL, NULL,
                 POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,
                 &ble_toggle_driver_api);
