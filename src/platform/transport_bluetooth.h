#pragma once

#include <furi.h>

typedef struct TransportBluetooth TransportBluetooth;

TransportBluetooth* transport_bluetooth_alloc(FuriMessageQueue* link_queue);

void transport_bluetooth_free(TransportBluetooth* transport);
