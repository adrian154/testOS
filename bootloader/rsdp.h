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

struct RSDPDescriptor *RSDP;
struct RSDPDescriptor2 *RSDP2;

extern bool findRSDP();
