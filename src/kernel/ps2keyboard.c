#include "ps2keyboard.h"
#include "irq.h"
#include "idt.h"
#include "ps2.h"

/* Keyboard driver runs on simple finite state machine model */

/* Private defines */
#define STATE_DONE          0
#define STATE_AFTER_E0      1
#define STATE_AFTER_F0      2
#define STATE_AFTER_E0F0    3

#define ACTION_NONE         0
#define ACTION_PRESS        1
#define ACTION_RELEASE      2

/* Key states. */
bool keyStates[256];
int state;

void handleKeyboardIRQ(struct InterruptFrame *frame) {

    uint8 scancode = inb(PS2_DATA_PORT);

    if(state == STATE_DONE) {

        if(scancode == 0xE0)
            state = STATE_AFTER_E0;
        else if(scancode == 0xF0)
            state = STATE_AFTER_F0;

    } else if(state == STATE_AFTER_E0) {

        if(scancode == 0xF0)
            state = STATE_AFTER_E0F0;
        
    } else if(state == STATE_AFTER_F0) {

    } else if(state == STATE_AFTER_E0F0) {

    }

}

void initKeyboard() {

    state = STATE_DONE;
    installIRQHandler(IRQ_PS2_KEYBOARD, handleKeyboardIRQ);

}