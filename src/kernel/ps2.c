#include "ps2.h"
#include "misc.h"
#include "types.h"
#include "textmode.h"

/* Bitmasks for config byte */
#define CFG_PORT1_IRQ       0b00000001
#define CFG_PORT2_IRQ       0b00000010
#define CFG_KB_TRANSLATE    0b01000000

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

    /* Update controller configuration byte */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_READ_CONFIG_BYTE);
    ps2_waitRead();
    uint8 configByte = inb(PS2_DATA_PORT);
    
    /* Enable IRQs, unset translation */
    configByte |= CFG_PORT1_IRQ | CFG_PORT2_IRQ;
    if((configByte >> 6) & 1) configByte ^= 0b01000000;

    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_WRITE_CONFIG_BYTE);
    ps2_waitWrite();
    outb(PS2_DATA_PORT, configByte);

    /* Reenable devices */
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT1);
    ps2_waitWrite();
    outb(PS2_COMMAND_REGISTER, PS2_ENABLE_PORT2);

    return true;

}