#include <pic/pic.h>
#include <intrins.h>
#include <util/interrupt_lock.h>

enum
{
    PIC_MasterCommand = 0x20,
    PIC_MasterData = 0x21,
    PIC_SlaveCommand = 0xA0,
    PIC_SlaveData = 0xA1,

    ICW1_ICW4 = 0x01,      /* ICW4 (not) needed */
    ICW1_SINGLE = 0x02,    /* Single (cascade) mode */
    ICW1_INTERVAL4 = 0x04, /* Call address interval 4 (8) */
    ICW1_LEVEL = 0x08,     /* Level triggered (edge) mode */
    ICW1_INIT = 0x10,      /* Initialization - required! */

    ICW4_8086 = 0x01,       /* 8086/88 (MCS-80/85) mode */
    ICW4_AUTO = 0x02,       /* Auto (normal) EOI */
    ICW4_BUF_SLAVE = 0x08,  /* Buffered mode/slave */
    ICW4_BUF_MASTER = 0x0C, /* Buffered mode/master */
    ICW4_SFNM = 0x10,       /* Special fully nested (not) */
};

//#include<debug/debug.h>

static interrupt_lock lock;

void PIC8259::initialize(uint8_t IRQStartVector)
{
    //debug <<"IRQ on " << (int)IRQStartVector << "\n";
    lock.lock();
    outbyte(PIC_MasterCommand, ICW1_INIT | ICW1_ICW4); //ICW1
    outbyte(PIC_MasterData, IRQStartVector);           //ICW2 : IRQ start vector must be multiple of 8.                                      
    outbyte(PIC_MasterData, 1 << 2);                   //ICW3 : tell slave PIC IRQ.
    outbyte(PIC_MasterData, ICW4_8086);                //ICW4

    outbyte(PIC_SlaveCommand, ICW1_INIT | ICW1_ICW4);
    outbyte(PIC_SlaveData, IRQStartVector + 8);
    outbyte(PIC_SlaveData, 2);//IRQ2
    outbyte(PIC_SlaveData, ICW4_8086);
    lock.unlock();
}

void PIC8259::maskInterrupt(uint16_t IRQBitmask)
{
    lock.lock();
    outbyte(PIC_MasterData, (uint8_t)(IRQBitmask & 0xff));
    outbyte(PIC_SlaveData, (uint8_t)(IRQBitmask >> 8));
    lock.unlock();
}
uint16_t PIC8259::getMask(){
    lock.lock();
    auto ret = inbyte(PIC_MasterData) | (inbyte(PIC_SlaveData) << 8);
    lock.unlock();
    return ret;
}

void PIC8259::sendEOI(unsigned IRQNumber)
{
    lock.lock();
    outbyte(PIC_MasterCommand, 0x20);
    if (8 <= IRQNumber)
        outbyte(PIC_SlaveCommand, 0x20);
    lock.unlock();
}