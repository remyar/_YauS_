#ifndef _ARCH_USB_H_
#define _ARCH_USB_H_

#include "arch.h"
#include "./lib/USB_DEVICE/usb_device.h"

void ARCH_UsbInit(void);
uint8_t ARCH_UsbRead(void);
bool ARCH_UsbAvailable(void);
uint8_t ARCH_UsbSend(uint8_t *Buf, uint16_t Len);
uint32_t ARCH_UsbGetBaudrate(void);
#endif