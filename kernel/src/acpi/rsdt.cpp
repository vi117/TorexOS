#include <acpi/rsdt.h>
#include <stringutil.h>

static constexpr phys_addr_t ebda_location = physical_location(0x40e);
static constexpr phys_addr_t bios_area = physical_location(0xe0000);
static constexpr size_t bios_area_length = (0xfffff - 0xe0000);
const acpi::RSDPDescriptor * acpi::findRSDP(){
    const uintptr_t ebda_seg = *ebda_location.to_ker().to_ptr_of<uint16_t>();
    static const char signature[] = "RSD PTR ";
    const char * ebda = physical_location(ebda_seg << 4).to_ker().to_ptr_of<const char>();
    for(size_t i = 0; i < 1_KB; i += 16){
        if(text::memcmp(&ebda[i],signature,8) == 0)
        {
            return reinterpret_cast<const RSDPDescriptor *>(&ebda[i]);
        }
    }
    const char * const bios = bios_area.to_ker().to_ptr_of<char>();
    for(size_t i = 0; i < bios_area_length; i += 16){
        if(text::memcmp(&bios[i],signature,8) == 0)
        {
            return reinterpret_cast<const RSDPDescriptor *>(&bios[i]);
        }
    }
    return nullptr;
}

bool acpi::doCheckSum(SDTHeader * tableHeader)
{
	uint8_t sum = 0;

	for (size_t i = 0; i < tableHeader->Length; i++)
	{
		sum += ((char *)tableHeader)[i];
	}

	return (sum == 0);
}
static const acpi::RSDPDescriptor * rsd_ptr = nullptr;
static acpi::FADT * fadt_ptr = nullptr;
static acpi::MADT * madt_ptr = nullptr;

bool acpi::prepareTable()
{
    rsd_ptr = findRSDP();
    if(rsd_ptr == nullptr)
        return false;
    fadt_ptr = (FADT *)findSDT("FACP");
    madt_ptr = (MADT *)findSDT("APIC");
    return true;
}

void acpi::printSDTs(text::raw_ostream & os){
    if(rsd_ptr == nullptr)
        prepareTable();
    if (rsd_ptr->Revision < 2){
        auto rsdt = physical_location(rsd_ptr->RsdtAddress)
            .to_ker().to_ptr_of<acpi::RSDT>();
        for(size_t i = 0; i < rsdt->entry_count(); i++){
            auto header = physical_location(rsdt->other_sdt_ptr[i])
                .to_ker().to_ptr_of<acpi::SDTHeader>();
            os.write(header->Signature,4);
            os << "\n";
        }
    }
    else
    {
        auto xsdp = (RSDPDescriptor20 *)rsd_ptr;
        auto xsdt = xsdp->XsdtAddress.to_ker().to_ptr_of<acpi::XSDT>();
        for(size_t i = 0; i < xsdt->entry_count(); i++){
            auto header = xsdt->other_sdt_ptr[i]
                .to_ker().to_ptr_of<acpi::SDTHeader>();
            os.write(header->Signature,4);
            os << "\n";
        }
    }
}

acpi::SDTHeader *  acpi::findSDT(const char * name)
{
    if(rsd_ptr == nullptr)
        prepareTable();
    
    if (rsd_ptr->Revision < 2){
        auto rsdt = physical_location(rsd_ptr->RsdtAddress)
            .to_ker().to_ptr_of<acpi::RSDT>();
        for(size_t i = 0; i < rsdt->entry_count(); i++){
            auto header = physical_location(rsdt->other_sdt_ptr[i])
                .to_ker().to_ptr_of<acpi::SDTHeader>();
            if( *(uint32_t *)(&header->Signature[0]) == *(uint32_t *)(&name[0])){
                return header;
            }
        }
    }
    else
    {
        auto xsdp = (RSDPDescriptor20 *)rsd_ptr;
        auto xsdt = xsdp->XsdtAddress.to_ker().to_ptr_of<acpi::XSDT>();
        for(size_t i = 0; i < xsdt->entry_count(); i++){
            auto header = xsdt->other_sdt_ptr[i]
                .to_ker().to_ptr_of<acpi::SDTHeader>();
            if( *(uint32_t *)(&header->Signature[0]) == *(uint32_t *)(&name[0])){
                return header;
            }
        }
    }
    return nullptr;
}

acpi::FADT * acpi::findFADT(){
    if(rsd_ptr == nullptr)
        prepareTable();
    return fadt_ptr;
}
acpi::MADT * acpi::findMADT(){
    if(rsd_ptr == nullptr)
        prepareTable();
    return madt_ptr;
}