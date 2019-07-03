#include "bool.h"

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

struct RSDPDescriptor *RSDP;
struct RSDPDescriptor2 *RSDP2;
struct RSDT *RSDT;

extern bool findRSDP();
extern bool initRSDT();
