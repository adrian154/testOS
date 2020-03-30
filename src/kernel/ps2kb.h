#ifndef __PS2KB_H
#define __PS2KB_H

#include "ps2kb.h"
#include "keys.h"

struct Keystroke {
    enum key key;
    bool state;
};

typedef void (*KeystrokeHandler)(struct Keystroke keystroke);

extern bool initKeyboard();
extern void setKeystrokeHandler(KeystrokeHandler keystrokeHandler);
extern void clearKeystrokeHandler();

#endif