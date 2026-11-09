#include "transport_hid.h"

#include <furi_hal_usb_hid.h>

#include "transport_bluetooth.h"

struct Transport {
    TransportBluetooth* bluetooth;
};

static const uint16_t transport_hid_keys[TransportKeyMAX] = {
    [TransportKeyArrowDown] = HID_KEYBOARD_DOWN_ARROW,
    [TransportKeyArrowUp] = HID_KEYBOARD_UP_ARROW,
    [TransportKeyControlF] = KEY_MOD_LEFT_CTRL | HID_KEYBOARD_F,
    [TransportKeyControlL] = KEY_MOD_LEFT_CTRL | HID_KEYBOARD_L,
    [TransportKeyControlQ] = KEY_MOD_LEFT_CTRL | HID_KEYBOARD_Q,
    [TransportKeyControlX] = KEY_MOD_LEFT_CTRL | HID_KEYBOARD_X,
    [TransportKeyEnter] = HID_KEYBOARD_RETURN,
    [TransportKeyEsc] = HID_KEYBOARD_ESCAPE,
};

Transport* transport_hid_alloc(FuriMessageQueue* link_queue) {
    Transport* transport = malloc(sizeof(Transport));
    transport->bluetooth = transport_bluetooth_alloc(link_queue);
    return transport;
}

void transport_hid_free(Transport* transport) {
    transport_bluetooth_free(transport->bluetooth);
    free(transport);
}

void transport_press(Transport* transport, TransportKey key) {
    transport_bluetooth_press(transport->bluetooth, transport_hid_keys[key]);
}

void transport_release(Transport* transport, TransportKey key) {
    transport_bluetooth_release(transport->bluetooth, transport_hid_keys[key]);
}

void transport_offer(Transport* transport) {
    transport_bluetooth_offer(transport->bluetooth);
}

void transport_withdraw(Transport* transport) {
    transport_bluetooth_withdraw(transport->bluetooth);
}
