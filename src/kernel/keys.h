#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "bool.h"
#include "ps2.h"
#include "types.h"

/*
#define KEY_ESC             0x01
#define KEY_F1              0x02
#define KEY_F2              0x03
#define KEY_F3              0x04
#define KEY_F4              0x05
#define KEY_F5              0x06
#define KEY_F6              0x07
#define KEY_F7              0x08
#define KEY_F8              0x09
#define KEY_F9              0x0A
#define KEY_F10             0x0B
#define KEY_F11             0x0C
#define KEY_F12             0x0D
#define KEY_PRINTSCREEN     0x0E
#define KEY_SCROLLOCK       0x0F
#define KEY_PAUSEBREAK      0x10
#define KEY_BACKTICK        0x11
#define KEY_0               0x12
#define KEY_1               0x13
#define KEY_2               0x14
#define KEY_3               0x15
#define KEY_4               0x16
#define KEY_5               0x17
#define KEY_6               0x18
#define KEY_7               0x19
#define KEY_8               0x1A
#define KEY_9               0x1B
#define KEY_MINUS           0x1C
#define KEY_EQUALS          0x1D
#define KEY_BACKSPACE       0x1E
#define KEY_TAB             0x1F
#define KEY_A               0x20
#define KEY_B               0x21
#define KEY_C               0x22
#define KEY_D               0x23
#define KEY_E               0x24
#define KEY_F               0x25
#define KEY_G               0x26
#define KEY_H               0x27
#define KEY_I               0x28
#define KEY_J               0x29
#define KEY_K               0x2A
#define KEY_L               0x2B
#define KEY_M               0x2C
#define KEY_N               0x2D
#define KEY_O               0x2E
#define KEY_P               0x2F
#define KEY_Q               0x30
#define KEY_R               0x31
#define KEY_S               0x32
#define KEY_T               0x33
#define KEY_U               0x34
#define KEY_V               0x35
#define KEY_W               0x36
#define KEY_X               0x37
#define KEY_Y               0x38
#define KEY_Z               0x39
#define KEY_LEFTBRACKET     0x3A
#define KEY_RIGHTBRACKET    0x3B
#define KEY_BACKSLASH       0x3C
#define KEY_SLASH           0x3D
#define KEY_CAPSLOCK        0x3E
#define KEY_SEMICOLON       0x3F
#define KEY_QUOTE           0x40
#define KEY_COMMA           0x41
#define KEY_PERIOD          0x42
#define KEY_ENTER           0x43
#define KEY_LSHIFT          0x44
#define KEY_RSHIFT          0x45
#define KEY_LCTRL           0x46
#define KEY_RCTRL           0x47
#define KEY_LALT            0x48
#define KEY_RALT            0x49
#define KEY_LMEDIA          0x4A
#define KEY_RMEDIA          0x4B
#define KEY_MENUS           0x4C
#define KEY_HOME            0x4D
#define KEY_PAGEUP          0x4E
#define KEY_PAGEDOWN        0x4F
#define KEY_DELETE          0x50
#define KEY_NUMPAD_STAR     0x51
#define KEY_NUMPAD_PLUS     0x52
#define KEY_NUMLOCK         0x53
#define KEY_UP              0x54
#define KEY_DOWN            0x55
#define KEY_LEFT            0x56
#define KEY_RIGHT           0x57
#define KEY_INSERT          0x58
#define KEY_SPACE           0x59
*/

enum key {
    KEY_NONE,
	KEY_ESC,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_BACKTICK,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_DASH,
	KEY_EQUALS,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_BACKSLASH,
	KEY_SEMICOLON,
	KEY_QUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_LCTRL,
	KEY_RCTRL,
	KEY_LALT,
	KEY_RALT,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LWIN,
	KEY_RWIN,
	KEY_DELETE,
	KEY_BACKSPACE,
	KEY_MENUS,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_PRINTSCREEN,
	KEY_SCROLLOCK,
	KEY_PAUSE,
	KEY_INSERT,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_END,
	KEY_KP_NUMLOCK,
	KEY_KP_SLASH,
	KEY_KP_STAR,
	KEY_KP_DASH,
	KEY_KP_PLUS,
	KEY_KP_ENTER,
	KEY_KP_PERIOD,
	KEY_KP_0,
	KEY_KP_1,
	KEY_KP_2,
	KEY_KP_3,
	KEY_KP_4,
	KEY_KP_5,
	KEY_KP_6,
	KEY_KP_7,
	KEY_KP_8,
	KEY_KP_9,
    KEY_ENTER,
    KEY_SPACE
};

#endif