#include "arch_usb.h"

static bool useUsb = false;

void ARCH_UsbInit(void)
{
    useUsb = true;
    MX_USB_DEVICE_Init();
}

bool ARCH_UseUsb(void)
{
    return useUsb;
}