/*
 * boot.h
 *
 *  Created on: 2013-3-21
 *      Author: BorisJineman
 */

#ifndef BOOT_H_
#define BOOT_H_

#define NT_DEVICE_NAME      L"\\Device\\GIRAFFEOS"
#define DOS_DEVICE_NAME     L"\\DosDevices\\GiraffeOS"

#define CMOS_WRITE(address, value) { WRITE_PORT_UCHAR((PUCHAR)0x70, address); WRITE_PORT_UCHAR((PUCHAR)0x71, value); }

#endif /* BOOT_H_ */




