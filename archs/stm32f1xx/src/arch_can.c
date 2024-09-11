#include "arch_can.h"

static s_CAN_DECODER sCanDecoder[CAN1_DECODER_FUNC_SIZE];
static uint16_t sCanDecoderIdx = 0;

static uint8_t _extIDs = 0;
static bool useCan = false;

// Register addresses
#define _CANBase 0x40006400

const static uint32_t mcr = _CANBase + 0x000;  // master cntrl
const static uint32_t msr = _CANBase + 0x004;  // rx status
const static uint32_t tsr = _CANBase + 0x008;  // tx status
const static uint32_t rf0r = _CANBase + 0x00C; // rx fifo 0 info reg

const static uint32_t ier = _CANBase + 0x014; // interrupt enable

const static uint32_t btr = _CANBase + 0x01C; // bit timing and rate

const static uint32_t ti0r = _CANBase + 0x180;  // tx mailbox id
const static uint32_t tdt0r = _CANBase + 0x184; // tx data len and time stamp
const static uint32_t tdl0r = _CANBase + 0x188; // tx mailbox data[3:0]
const static uint32_t tdh0r = _CANBase + 0x18C; // tx mailbox data[7:4]

const static uint32_t ri0r = _CANBase + 0x1B0;  // rx fifo id reg
const static uint32_t rdt0r = _CANBase + 0x1B4; // fifo data len and time stamp
const static uint32_t rdl0r = _CANBase + 0x1B8; // rx fifo data low
const static uint32_t rdh0r = _CANBase + 0x1BC; // rx fifo data high

const static uint32_t fmr = _CANBase + 0x200;   // filter master reg
const static uint32_t fm1r = _CANBase + 0x204;  // filter mode reg
const static uint32_t fs1r = _CANBase + 0x20C;  // filter scale reg, 16/32 bits
const static uint32_t ffa1r = _CANBase + 0x214; // filter FIFO assignment
const static uint32_t fa1r = _CANBase + 0x21C;  // filter activation reg
const static uint32_t fr1 = _CANBase + 0x240;   // id/mask acceptance reg1
const static uint32_t fr2 = _CANBase + 0x244;   // id/mask acceptance reg2

#define _scsBase 0xE000E000UL                   // System Control Space Base Address
#define _nvicBase (_scsBase + 0x0100UL)         // NVIC Base Address
const static uint32_t iser = _nvicBase + 0x000; //  NVIC interrupt set (enable)
const static uint32_t icer = _nvicBase + 0x080; // NVIC interrupt clear (disable)

const static uint32_t scbBase = _scsBase + 0x0D00UL;
const static uint32_t vtor = _scsBase + 0x008;

// GPIO/AFIO Regs
#define _afioBase 0x40010000UL
const static uint32_t mapr = _afioBase + 0x004; // alternate pin function mapping

#define _gpioABase 0x40010800UL                  // port A
const static uint32_t crhA = _gpioABase + 0x004; // cntrl reg for port b
const static uint32_t odrA = _gpioABase + 0x00c; // output data reg

#define _gpioBBase (_gpioABase + 0x400)          // port B
const static uint32_t crhB = _gpioBBase + 0x004; // cntrl reg for port b
const static uint32_t odrB = _gpioBBase + 0x00c; // output data reg

// Clock
#define _rcc 0x40021000UL
#define _rccBase 0x40021000UL
const static uint32_t apb1enr = _rccBase + 0x01c;
const static uint32_t apb2enr = _rccBase + 0x018;

// Helpers
#define MMIO32(x) (*(volatile uint32_t *)(x))
#define MMIO16(x) (*(volatile uint16_t *)(x))
#define MMIO8(x) (*(volatile uint8_t *)(x))

#define INRQ mcr, 0
#define INAK msr, 0
#define FINIT fmr, 0
#define fmpie0 1 // rx interrupt enable on rx msg pending bit

static inline volatile void periphSetBit(uint32_t addr, int bitNum, bool val) // peripheral bit tool
{
    MMIO32(0x42000000 + ((addr & 0xFFFFF) << 5) + (bitNum << 2)) = val; // uses bit band memory
}

static inline volatile bool periphGetBit(uint32_t addr, int bitNum) // peripheral bit tool
{
    return MMIO32(0x42000000 + ((addr & 0xFFFFF) << 5) + (bitNum << 2)); // uses bit band memory
}

void _filter16Init(int bank, int mode, int a, int b, int c, int d) // 16b filters
{
    periphSetBit(FINIT, 1);                          // FINIT  'init' filter mode ]
    periphSetBit(fa1r, bank, 0);                     // de-activate filter 'bank'
    periphSetBit(fs1r, bank, 0);                     // fsc filter scale reg,  0 => 2ea. 16b
    periphSetBit(fm1r, bank, mode);                  // fbm list mode = 1, 0 = mask
    MMIO32(fr1 + (8 * bank)) = (b << 21) | (a << 5); // fltr1,2 of flt bank n  OR  flt/mask 1 in mask mode
    MMIO32(fr2 + (8 * bank)) = (d << 21) | (c << 5); // fltr3,4 of flt bank n  OR  flt/mask 2 in mask mode
    periphSetBit(fa1r, bank, 1);                     // activate this filter ]
    periphSetBit(FINIT, 0);                          // ~FINIT  'active' filter mode ]
}

void _filterList16Init(int bank, int idA, int idB, int idC, int idD) // 16b list filters
{
    _filter16Init(bank, 1, idA, idB, idC, idD); // fltr 1,2,3,4 of flt bank n
}

void _filterMask16Init(int bank, int idA, int maskA, int idB, int maskB) // 16b mask filters
{
    _filter16Init(bank, 0, idA, maskA, idB, maskB); // fltr 1,2 of flt bank n
}

static void _begin(uint8_t addrType, int brp, bool singleWire, bool alt, bool pullup)
{
    uint8_t inp_float = 0b0100;
    uint8_t inp_pull = 0b1000;
    uint8_t alt_out = 0b1001;
    uint8_t alt_out_od = 0b1101;

    _extIDs = addrType;

    // set up CAN IO pins
    uint8_t swMode = singleWire ? alt_out_od : alt_out;
    uint8_t inputMode = pullup ? inp_pull : inp_float;

    if (alt)
    {
        MMIO32(apb2enr) |= (1 << 3) | (1 << 0); // enable gpioB = b3 and afio = b0 clks
        MMIO32(mapr) |= (2 << 13);              // alt func, CAN remap to B9+B8
        MMIO32(crhB) &= 0xFFFFFF00;             // clear control bits for pins 8 & 9 of Port B
        MMIO32(crhB) |= inputMode;              // pin8 for rx, b0100 = b01xx, floating, bxx00 input
        periphSetBit(odrB, 8, pullup);          // = true;           // set input will pullup resistor for single wire with pullup mode
        MMIO32(crhB) |= swMode << 4;            // set output
    }
    else
    {
        MMIO32(apb2enr) |= (1 << 2) | (1 << 0); // enable gpioA = b2 and afio = b0 clks
        MMIO32(mapr) &= 0xffff9fff;             // CAN map to default pins, PA11/12
        MMIO32(crhA) &= 0xFFF00FFF;             // clear control bits for pins 11 & 12 of Port A
        MMIO32(crhA) |= inputMode << 12;        // pin11 for rx, b0100 = b01xx, floating, bxx00 input
        periphSetBit(odrA, 11, pullup);         //
        MMIO32(crhA) |= swMode << 16;           // set output
    }

    // set up CAN peripheral
    periphSetBit(_rcc + 0x1C, 25, 1);   // enable CAN1
    periphSetBit(mcr, 1, 0);            // exit sleep
    MMIO32(mcr) |= (1 << 6) | (1 << 0); // set ABOM, init req (INRQ)

    while (periphGetBit(INAK) == 0) // wait for hw ready
        ;

    MMIO32(btr) = (3 << 20) | (12 << 16) | (brp << 0); // 125K, 12/15=80% sample pt. prescale = 15
    // periphBit(ti0r, 2) = _extIDs;                      // 0 = std 11b ids, 1 = extended 29b ids
    periphSetBit(INRQ, 0);     // request init leave to Normal mode
    while (periphGetBit(INAK)) // wait for hw
        ;

    _filterMask16Init(0, 0, 0, 0, 0); // let all msgs pass to fifo0 by default

    periphSetBit(ier, fmpie0, 1); // set fifo RX int enable request
    MMIO32(iser) = 1UL << 20;
}

void ARCH_CanInit(uint32_t periphNum, unsigned long baud, uint32_t flags)
{
    bool alt = false;  // default or alternate pins
    bool wire = false; // bus uses a xcvr chip
    bool pullUp = true;

    switch (periphNum)
    {
    case CAN_1:
        /* code */
        if (Has_flag(flags, PORTA_11_12_XCVR))
        {
            alt = false;
            wire = false;
        }
        else if (Has_flag(flags, PORTB_8_9_XCVR))
        {
            alt = true;
            wire = false;
        }
        else if (Has_flag(flags, PORTA_11_12_WIRE))
        {
            alt = false;
            wire = true;
        }
        else if (Has_flag(flags, PORTB_8_9_WIRE))
        {
            alt = true;
            wire = true;
        }
        else if (Has_flag(flags, PORTA_11_12_WIRE_PULLUP))
        {
            alt = false;
            wire = true;
            pullUp = true;
        }
        else if (Has_flag(flags, PORTB_8_9_WIRE_PULLUP))
        {
            alt = true;
            wire = true;
            pullUp = true;
        }
        break;

    default:
        break;
    }

    for (uint16_t i = 0; i < CAN1_DECODER_FUNC_SIZE; i++)
    {
        sCanDecoder[i].id = NULL;
        sCanDecoder[i].decoderFunc = NULL;
    }

    useCan = true;

    _begin(Has_flag(flags, STD_ID_LEN) ? 0 : 1, baud, wire, alt, pullUp);
}

bool ARCH_UseCan(void)
{
    return useCan;
}

int _receive(volatile int *id, volatile int *fltrIdx, volatile uint8_t *pData)
{
    int len = -1;
    bool _rxExtended;
    // rxMsgCnt = MMIO32(rf0r) & (3 << 0); //num of msgs
    // rxFull = MMIO32(rf0r) & (1 << 3);
    // rxOverflow = MMIO32(rf0r) & (1 << 4); // b4

    if (MMIO32(rf0r) & (3 << 0)) // num of msgs pending
    {
        _rxExtended = ((MMIO32(ri0r) & 1 << 2) >> 2);

        if (_rxExtended)
            *id = (MMIO32(ri0r) >> 3); // extended id
        else
            *id = (MMIO32(ri0r) >> 21);         // std id
        len = MMIO32(rdt0r) & 0x0F;             // fifo data len and time stamp
        *fltrIdx = (MMIO32(rdt0r) >> 8) & 0xff; // filter match index. Index accumalates from start of bank
        ((uint32_t *)pData)[0] = MMIO32(rdl0r); // 4 low rx bytes
        ((uint32_t *)pData)[1] = MMIO32(rdh0r); // another 4 bytes
        periphSetBit(rf0r, 5, 1);               // release the mailbox*/
    }
    return len;
}

void _HAL_CAN_IRQHandler(void)
{
    int id, fltIdx, len;
    uint8_t data[8];

    while (MMIO32(rf0r) & (3 << 0))
    {
        len = _receive(&id, &fltIdx, data);
        if (len >= 0)
        {
            for (uint16_t i = 0; i < CAN1_DECODER_FUNC_SIZE; i++)
            {
                if ((sCanDecoder[i].id != NULL) && (sCanDecoder[i].decoderFunc != NULL))
                {
                    if (sCanDecoder[i].id == id)
                    {
                        sCanDecoder[i].decoderFunc(data);
                    }
                }
            }
        }
    }
}

void ARCH_CanAddIsr(uint16_t id, CAN_DECODER_FUNC ptrFunc)
{
    for (uint16_t i = 0; i < CAN1_DECODER_FUNC_SIZE; i++)
    {
        if ((sCanDecoder[i].id == NULL) && (sCanDecoder[i].decoderFunc == NULL))
        {
            sCanDecoder[i].id = id;
            sCanDecoder[i].decoderFunc = ptrFunc;
            sCanDecoderIdx++;
            break;
        }
    }
}