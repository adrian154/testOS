#ifndef __ACPI_H
#define __ACPI_H

#include "bool.h"

/* For use with GAS */
#define ADDR_SPACE_MEMORY	0
#define ADDR_SPACE_IO		1

struct RSDPDescriptor {
	char signature[8];
	unsigned char checksum;
	char OEMID[6];
	unsigned char revision;
	unsigned int RSDTAddress;
}__attribute__((packed));

struct RSDPDescriptor2 {
	struct RSDPDescriptor oldDescriptor;
	unsigned int length;
	unsigned long XSDTAddress;
	unsigned char extendedChecksum;
	unsigned char reserved[3];
}__attribute__((packed));

struct SDTHeader {
	char signature[4];
	unsigned int length;
	unsigned char revision;
	unsigned char checksum;
	char OEMID[6];
	char OEMTableID[8];
	unsigned int OEMRevision;
	unsigned int creatorID;
	unsigned int creatorRevision;
}__attribute__((packed));

struct RSDT {
	struct SDTHeader header;
	unsigned int otherTables;
}__attribute__((packed));

/* "GAS" (Generic Address Structure) */
struct ACPIAddress {
	unsigned char addressSpace;
	unsigned char registerWidth;
	unsigned char registerOffset;
	unsigned char reserved;
	unsigned long address;
};

struct RSDPDescriptor *RSDP;
struct RSDPDescriptor2 *RSDP2;
struct RSDT *RSDT;

extern bool findRSDP();
extern bool initRSDT();
extern struct SDTHeader *findTable(char signature[4]);

#endif
