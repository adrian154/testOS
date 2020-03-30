#ifndef __PS2_H
#define __PS2_H

#include "bool.h"

#define PS2_DATA_PORT           0x60
#define PS2_STATUS_REGISTER     0x64
#define PS2_COMMAND_REGISTER    0x64

/* PS2 controller commands */
#define PS2_READ_CONFIG_BYTE    0x20
#define PS2_WRITE_CONFIG_BYTE   0x60
#define PS2_DISABLE_PORT1       0xAD
#define PS2_DISABLE_PORT2       0xA7
#define PS2_ENABLE_PORT1        0xAE
#define PS2_ENABLE_PORT2        0xA8
#define PS2_SELF_TEST           0xAA

/* PS2 controller special bytes */
#define PS2_SELF_TEST_OK        0x55

/* PS2 keyboard commands */
#define PS2_KB_SET_LED             0xED
#define PS2_KB_SET_SCANCODE_SET    0xF0
#define PS2_KB_ENABLE_SCANNING     0xF4
#define PS_KB_DISABLE_SCANNING     0xF5
#define PS2_KB_RESET               0xFF

/* PS2 keyboard special bytes */
#define PS2_KB_ERROR1              0x00
#define PS2_KB_SELF_TEST_OK        0x55
#define PS2_KB_ECHO                0xEE
#define PS2_KB_ACK                 0xFA
#define PS2_KB_SELF_TEST_FAILED    0xFC
#define PS2_KB_RESEND              0xFE
#define PS2_KB_ERROR2              0xFF

#define PS2_KB_LED_SCRLLOCK        0b001
#define PS2_KB_LED_NUMLOCK         0b010
#define PS2_KB_LED_CAPSLOCK        0b100

#define PS2_KB_SCANCODE_SET_1      1
#define PS2_KB_SCANCODE_SET_2      2
#define PS2_KB_SCANCODE_SET_3      3

extern bool initPS2Controller();
extern void ps2_waitWrite();
extern void ps2_waitRead();
extern void ps2_waitAck();

#endif 