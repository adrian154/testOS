#ifndef __ACPI_H
#define __ACPI_H

#include "bool.h"
#include "types.h"

/* For use with GAS */
#define ADDR_SPACE_MEMORY	0
#define ADDR_SPACE_IO		1

struct RSDPDescriptor {
	char signature[8];
	uint8 checksum;
	char OEMID[6];
	uint8 revision;
	uint32 RSDTAddress;
}__attribute__((packed));

struct SDTHeader {
	char signature[4];
	uint32 length;
	uint8 revision;
	uint8 checksum;
	char OEMID[6];
	char OEMTableID[8];
	uint32 OEMRevision;
	uint32 creatorID;
	uint32 creatorRevision;
}__attribute__((packed));

struct RSDT {
	struct SDTHeader header;
	uint32 otherTables;
}__attribute__((packed));

/* "GAS" (Generic Address Structure) */
struct ACPIAddress {
	uint8 addressSpace;
	uint8 registerWidth;
	uint8 registerOffset;
	uint8 reserved;
	uint64 address;
};

struct RSDPDescriptor *RSDP;
struct RSDPDescriptor2 *RSDP2;
struct RSDT *RSDT;

extern bool findRSDP();
extern bool initRSDT();
extern struct SDTHeader *findTable(char signature[4]);

#endif
