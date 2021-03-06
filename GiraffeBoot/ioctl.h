/*
* ioctl.h
*
*  Created on: 2015-6-11
*		Author: Liu, Boris
*/

#ifndef IOCTL_H_
#define IOCTL_H_

//
// The IOCTL function codes from 0x800 to 0xFFF are for customer use.
//

#define IOCTL_GIRAFFEOS_METHOD_BUFFERED \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x902, METHOD_BUFFERED, FILE_ANY_ACCESS  )

#define IOCTL_GIRAFFEOS_METHOD_APPLY_SETTING \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x904, METHOD_BUFFERED , FILE_ANY_ACCESS  )

#define IOCTL_GIRAFFEOS_METHOD_SET_APPLICATION_BIN \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x905, METHOD_BUFFERED , FILE_ANY_ACCESS  )

#define IOCTL_GIRAFFEOS_METHOD_START_APPLICATION \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x906, METHOD_BUFFERED , FILE_ANY_ACCESS  )

#define IOCTL_GIRAFFEOS_METHOD_STOP_APPLICATION \
	CTL_CODE( FILE_DEVICE_UNKNOWN, 0x907, METHOD_BUFFERED , FILE_ANY_ACCESS  )

typedef struct _BootSetting
{
	unsigned long CPUID;
	unsigned long HeapMemory;
	unsigned long StackMemory;
} BootSetting, *PBootSetting;

#endif /* IOCTL_H_ */
