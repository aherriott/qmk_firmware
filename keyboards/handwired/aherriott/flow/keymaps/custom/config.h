// Copyright 2023 Alex Herriott (@aherriott)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define MASTER_RIGHT

// Define options
#define TAPPING_TERM 150
#define PERMISSIVE_HOLD
#define TAPPING_TERM_PER_KEY

#define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode.
#define SERIAL_USART_TX_PIN GP16   // USART TX pin
#define SERIAL_USART_RX_PIN GP17   // USART RX pin

#define SPI_DRIVER SPID0
#define SPI_SCK_PIN GP2
#define SPI_MOSI_PIN GP3
#define SPI_MISO_PIN GP4
#define POINTING_DEVICE_CS_PIN GP5

#define SPLIT_HAND_PIN GP28

// #define PMW33XX_CS_PIN GP5
