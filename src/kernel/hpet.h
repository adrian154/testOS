#ifndef __HPET_H
#define __HPET_H

#include "acpi.h"
#include "bool.h"

struct HPETDescriptor {
    struct SDTHeader header;
    unsigned char hardwareRevisionID;
    unsigned char attributes;
    unsigned short PCIVendorID;
    struct ACPIAddress address;
    unsigned char HPETNumber;
    unsigned short minimumTick;
    unsigned char pageProtection;
}__attribute__((packed));

extern bool initHPET();	

#endif