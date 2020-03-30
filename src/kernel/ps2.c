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

void ps2_waitAck() {
    while(inb(PS2_DATA_PORT) != 0xFA);
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
        ERROR("PS/2 controller self test failed\n");
        return false;
    }

    /* Reenable devices */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT1);
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT2);

    return true;

}