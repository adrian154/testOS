#include "keys.h"
#include "irq.h"
#include "idt.h"
#include "ps2.h"
#include "misc.h"
#include "textmode.h"
#include "ps2kb.h"

/*
* ISSUES:
*   - Pause, PrntScrn not supported
*/

/* Keyboard driver runs on simple finite state machine model */

/* Private defines */
#define STATE_DONE          0
#define STATE_AFTER_E0      1
#define STATE_AFTER_F0      2
#define STATE_AFTER_E0F0    3

KeystrokeHandler keystrokeHandler;

/* Key states. */
bool keyStates[256];

/* State of the IRQ handler */
int state;

/* Single byte press events */
uint8 singleScancodePress[170] = {
    0x76, KEY_ESC,
    0x05, KEY_F1,
    0x06, KEY_F2,
    0x04, KEY_F3,
    0x0C, KEY_F4,
    0x03, KEY_F5,
    0x0B, KEY_F6,
    0x83, KEY_F7,
    0x0A, KEY_F8,
    0x01, KEY_F9,
    0x09, KEY_F10,
    0x78, KEY_F11,
    0x07, KEY_F12,
    0x7E, KEY_SCROLLOCK,
    0x0E, KEY_BACKTICK,
    0x16, KEY_1,
    0x1E, KEY_2,
    0x26, KEY_3,
    0x25, KEY_4,
    0x2E, KEY_5,
    0x36, KEY_6,
    0x3D, KEY_7,
    0x3E, KEY_8,
    0x46, KEY_9,
    0x45, KEY_0,
    0x4E, KEY_DASH,
    0x55, KEY_EQUALS,
    0x66, KEY_BACKSPACE,
    0x0D, KEY_TAB,
    0x15, KEY_Q,
    0x1D, KEY_W,
    0x24, KEY_E,
    0x2D, KEY_R,
    0x2C, KEY_T,
    0x35, KEY_Y,
    0x3C, KEY_U,
    0x43, KEY_I,
    0x44, KEY_O,
    0x4D, KEY_P,
    0x54, KEY_LBRACKET,
    0x5B, KEY_RBRACKET,
    0x5D, KEY_BACKSLASH,
    0x58, KEY_CAPSLOCK,
    0x1C, KEY_A,
    0x1B, KEY_S,
    0x23, KEY_D,
    0x2B, KEY_F,
    0x34, KEY_G,
    0x33, KEY_H,
    0x3B, KEY_J,
    0x42, KEY_K,
    0x4B, KEY_L,
    0x4C, KEY_SEMICOLON,
    0x52, KEY_QUOTE,
    0x5A, KEY_ENTER,
    0x12, KEY_LSHIFT,
    0x1A, KEY_Z,
    0x22, KEY_X,
    0x21, KEY_C,
    0x2A, KEY_V,
    0x32, KEY_B,
    0x31, KEY_N,
    0x3A, KEY_M,
    0x41, KEY_COMMA,
    0x49, KEY_PERIOD,
    0x4A, KEY_SLASH,
    0x59, KEY_RSHIFT,
    0x14, KEY_LCTRL,
    0x11, KEY_LALT,
    0x29, KEY_SPACE,
    0x77, KEY_KP_NUMLOCK,
    0x7C, KEY_KP_STAR,
    0x7B, KEY_KP_DASH,
    0x6C, KEY_KP_7,
    0x75, KEY_KP_8,
    0x7D, KEY_KP_9,
    0x79, KEY_KP_PLUS,
    0x6B, KEY_KP_4,
    0x73, KEY_KP_5,
    0x74, KEY_KP_6,
    0x69, KEY_KP_1,
    0x72, KEY_KP_2,
    0x7A, KEY_KP_3,
    0x70, KEY_KP_0,
    0x71, KEY_KP_PERIOD
};

uint8 e0Press[34] = {
    0x1F, KEY_LWIN,
    0x14, KEY_RCTRL,
    0x27, KEY_RWIN,
    0x11, KEY_RALT,
    0x2F, KEY_MENUS,
    0x70, KEY_INSERT,
    0x6C, KEY_SCROLLOCK,
    0x7D, KEY_PAGEUP,
    0x71, KEY_DELETE,
    0x69, KEY_END,
    0x7A, KEY_PAGEDOWN,
    0x75, KEY_UP,
    0x6B, KEY_LEFT,
    0x72, KEY_DOWN,
    0x74, KEY_RIGHT,
    0x4A, KEY_KP_SLASH,
    0x5a, KEY_KP_ENTER
};

void handleKeyboardIRQ(struct InterruptFrame *frame) {

    uint8 scancode = inb(PS2_DATA_PORT);
    enum key keyPressed = KEY_NONE;
    enum key keyReleased = KEY_NONE;

    if(state == STATE_DONE) {

        /* Search for single-scancode presses. */
        /* All key releases are at least 2 bytes. */
        for(int i = 0; i < 85; i++) {
            uint8 scKey = singleScancodePress[i * 2];
            if(scKey == scancode) {
                keyPressed = singleScancodePress[i * 2 + 1];
            }
        }

        if(scancode == 0xE0)
            state = STATE_AFTER_E0;
        else if(scancode == 0xF0)
            state = STATE_AFTER_F0;

    } else if(state == STATE_AFTER_E0) {

        for(int i = 0; i < 17; i++) {
            uint8 scKey = e0Press[i * 2];
            if(scKey == scancode) {
                keyPressed = e0Press[i * 2 + 1];
                state = STATE_DONE;
            }
        }

        if(scancode == 0xF0)
            state = STATE_AFTER_E0F0;
        
    } else if(state == STATE_AFTER_F0) {

        /* All after-F0 scancodes are single byte releases */
        /* `scancode` corresponds with single byte press scancode */
        for(int i = 0; i < 85; i++) {
            uint8 scKey = singleScancodePress[i * 2];
            if(scKey == scancode) {
                keyReleased = singleScancodePress[i * 2 + 1];
            }
        }

        /* Reset state, all F0(x) scancodes are 2-byte */
        state = STATE_DONE;

    } else if(state == STATE_AFTER_E0F0) {

        for(int i = 0; i < 17; i++) {
            uint8 scKey = e0Press[i * 2];
            if(scKey == scancode) {
                keyReleased = e0Press[i * 2 + 1];
                state = STATE_DONE;
            }
        }

    }

    /* Update key state, dispatch key events */
    keyStates[keyReleased] = false;
    keyStates[keyPressed] = true;

    if(keyReleased != KEY_NONE || keyPressed != KEY_NONE) {
        struct Keystroke keystroke;
        keystroke.key = (keyReleased != KEY_NONE ? keyReleased : keyPressed);
        keystroke.state = keyReleased != KEY_NONE ? false : true;
        keystrokeHandler(keystroke);
    }

}

bool initKeyboard() {

    state = STATE_DONE;
    for(int i = 0; i < 256; i++) {
        keyStates[i] = 0;
    }

    installIRQHandler(IRQ_PS2_KEYBOARD, handleKeyboardIRQ);
    return true;

}

void setKeystrokeHandler(KeystrokeHandler newHandler) {
    keystrokeHandler = newHandler;
}

void clearKeystrokeHandler() {
    keystrokeHandler = null;
}