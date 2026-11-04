#pragma once

typedef enum {
    TransportKeyControlF,
    TransportKeyControlQ,
    TransportKeyControlX,
    TransportKeyMAX,
} TransportKey;

typedef struct Transport Transport;

void transport_press(Transport* transport, TransportKey key);

void transport_release(Transport* transport, TransportKey key);
