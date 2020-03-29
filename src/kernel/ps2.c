#include "ps2.h"
#include "misc.h"
#include "types.h"
#include "textmode.h"

void ps2_waitRead() {
    while(!(inb(PS2_STATUS_REGISTER) & 0b0000001));
}

void ps2_waitWrite() {
    while(inb(PS2_STATUS_REGISTER) & 0b00000010);
}

bool initPS2Controller() {

    /* Disable all devices. */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_DISABLE_PORT1);
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_DISABLE_PORT2);
    
    /* Perform self test */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_SELF_TEST);
    ps2_waitRead();
    uint8 response = inb(PS2_DATA_PORT); 
    if(response != PS2_SELF_TEST_OK) {
        terminalForeground = BRIGHT_RED;
        printString("initPS2Keyboard(): controller self test failed (response was 0x"); printByte(response); printString(")\n");
        terminalForeground = WHITE;
        return false;
    }

    /* Reenable devices */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT1);
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT2);

    return true;

}