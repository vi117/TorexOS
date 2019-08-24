#pragma once
#include<stdint.h>
#include<memory/address.h>
#include<raw_ostream.h>

namespace acpi
{
	struct RSDPDescriptor {
		char Signature[8];
		uint8_t Checksum;
		char OEMID[6];
		uint8_t Revision;// if it's zero, use rsdt. otherwise, use xsdt.
		uint32_t RsdtAddress;
	};

	struct RSDPDescriptor20 : public RSDPDescriptor{
		uint32_t Length;
		phys_addr_t XsdtAddress;
		uint8_t ExtendedChecksum;
		uint8_t reserved[3];
	};

	struct SDTHeader {
		char Signature[4];
		uint32_t Length;
		uint8_t Revision;
		uint8_t Checksum;
		char OEMID[6];
		char OEMTableID[8];
		uint32_t OEMRevision;
		uint32_t CreatorID;
		uint32_t CreatorRevision;
	};

	struct RSDT : public SDTHeader{
		uint32_t other_sdt_ptr[0];
		size_t entry_count() const { return (Length - sizeof(SDTHeader)) / 4;}
	};
	struct XSDT : public SDTHeader{
		phys_addr_t other_sdt_ptr[0];
		size_t entry_count() const { return (Length - sizeof(SDTHeader)) / 8;}
	};

	const RSDPDescriptor * findRSDP();

	bool doCheckSum(SDTHeader *);

	SDTHeader * find(const char * name);
	void printSDTs(text::raw_ostream & os);
}