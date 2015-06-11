/*
 * boot.c
 *
 *  Created on: 2013-3-21
 *      Author: BorisJineman
 *     purpose: This code is currently just boot CPU1, written by BorisJineman.
 *     			More features will be supported later.
 *     			Let us wait and see.
 */


#include <ntddk.h>          // various NT definitions
#include <string.h>
#include "apic.h"
#include "ioctl.h"
#include "boot.h"


DRIVER_INITIALIZE DriverEntry;

__drv_dispatchType(IRP_MJ_CREATE)
__drv_dispatchType(IRP_MJ_CLOSE)
DRIVER_DISPATCH SioctlCreateClose;

__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)
DRIVER_DISPATCH SioctlDeviceControl;

DRIVER_UNLOAD UnloadGiraffeOS;

APIC * pAPIC = 0;
BootSetting * pBootSetting = 0;
void InitAPIC();
void StartCPU(int CPUID);

NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath)
{

	NTSTATUS        ntStatus;
	UNICODE_STRING  ntUnicodeString;    // NT Device Name "\Device\GIRAFFEOS"
	UNICODE_STRING  ntWin32NameString;    // Win32 Name "\DosDevices\GiraffeOS"
	PDEVICE_OBJECT  deviceObject = NULL;    // ptr to device object

	UNREFERENCED_PARAMETER(RegistryPath);

	RtlInitUnicodeString(&ntUnicodeString, NT_DEVICE_NAME);

	ntStatus = IoCreateDevice(
		DriverObject,                   // Our Driver Object
		0,                              // We don't use a device extension
		&ntUnicodeString,               // Device name "\Device\GIRAFFEOS"
		FILE_DEVICE_UNKNOWN,            // Device type
		FILE_DEVICE_SECURE_OPEN,     // Device characteristics
		FALSE,                          // Not an exclusive device
		&deviceObject);                // Returned ptr to Device Object

	if (!NT_SUCCESS(ntStatus))
	{
		//SIOCTL_KDPRINT(("Couldn't create the device object\n"));
		return ntStatus;
	}


	//
	// Initialize the driver object with this driver's entry points.
	//

	DriverObject->MajorFunction[IRP_MJ_CREATE] = SioctlCreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = SioctlCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = SioctlDeviceControl;
	DriverObject->DriverUnload = UnloadGiraffeOS;

	//
	// Initialize a Unicode String containing the Win32 name
	// for our device.
	//

	RtlInitUnicodeString(&ntWin32NameString, DOS_DEVICE_NAME);

	//
	// Create a symbolic link between our device name  and the Win32 name
	//

	ntStatus = IoCreateSymbolicLink(
		&ntWin32NameString, &ntUnicodeString);

	if (!NT_SUCCESS(ntStatus))
	{
		//
		// Delete everything that this routine has allocated.
		//
		//SIOCTL_KDPRINT(("Couldn't create symbolic link\n"));
		IoDeleteDevice(deviceObject);
	}

	return ntStatus;

}


NTSTATUS InitAPIC()
{
	unsigned long has_local_apic = 0;
	unsigned long pa = 0;
	PHYSICAL_ADDRESS PhysicalAddress;
	unsigned char * p_mem = 0;


	unsigned char ap_boot_code[149] = {
		0xFA, 0x31, 0xC0, 0x8E, 0xD8, 0x8E, 0xD0, 0xB8, 0x00, 0x80, 0x8E, 0xD8, 0xBB, 0x00, 0x10, 0xB9,
		0x64, 0x00, 0xB8, 0x00, 0x00, 0x89, 0x07, 0x43, 0x43, 0x40, 0xE2, 0xF9, 0xE5, 0x92, 0x83, 0xC8,
		0x02, 0xE7, 0x92, 0x31, 0xC0, 0x8E, 0xD8, 0x8E, 0xD0, 0xB8, 0x00, 0x80, 0x8E, 0xD8, 0x66, 0xB8,
		0x67, 0x00, 0x00, 0x00, 0x67, 0x0F, 0x01, 0x10, 0x0F, 0x20, 0xC0, 0x66, 0x83, 0xC8, 0x01, 0x0F,
		0x22, 0xC0, 0x66, 0xEA, 0x4A, 0x00, 0x08, 0x00, 0x08, 0x00, 0x66, 0xB8, 0x20, 0x00, 0x8E, 0xD8,
		0x8E, 0xC0, 0x8E, 0xE0, 0x8E, 0xE8, 0x8E, 0xD0, 0xBC, 0x00, 0x00, 0x00, 0x0F, 0xEB, 0xFE, 0xEA,
		0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xF4, 0x27, 0x00, 0x6D, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00, 0xFF, 0xFF, 0x00,
		0x00, 0x00, 0x92, 0xCF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xC8, 0x70, 0xFF, 0xFF, 0x00,
		0x00, 0x00, 0x92, 0xC8, 0x70
	};



	// detect local apic
	__asm {
		mov eax, 1;
		cpuid;
		shr edx, 9; // if there is a local apic, then bit 9 will be set
		and edx, 0x1;
		mov has_local_apic, edx;
	}

	if (!has_local_apic)
		return -1;

	pa = 0xFEE00000;
	__asm {
		mov ecx, 0x1B; // offset into apicbase
		rdmsr; //Load MSR specified by ECX into EDX:EAX
		mov edx, eax;
		and edx, 0xFFFFF000; // bits 31:12 contain the APICBASE address for the local apic
		mov pa, edx;
	}


	memset(&PhysicalAddress, 0, sizeof(PHYSICAL_ADDRESS));
	PhysicalAddress.LowPart = (unsigned long)pa;
	pAPIC = (APIC *)MmMapIoSpace(PhysicalAddress, sizeof(APIC), MmNonCached);



	memset(&PhysicalAddress, 0, sizeof(PHYSICAL_ADDRESS));
	PhysicalAddress.LowPart = (unsigned long)0x00080000;
	p_mem = (unsigned char *)MmMapIoSpace(PhysicalAddress, sizeof(ap_boot_code), MmNonCached);
	memcpy(p_mem, &ap_boot_code, sizeof(ap_boot_code));
	MmUnmapIoSpace(p_mem, sizeof(ap_boot_code));

	memset(&PhysicalAddress, 0, sizeof(PHYSICAL_ADDRESS));
	PhysicalAddress.LowPart = (unsigned long)0x000fffff;

	p_mem = (unsigned char *)MmAllocateContiguousMemory(0x00020000, PhysicalAddress);
	
	if (!p_mem)
		return -1;

	// make sure the align
	p_mem = (unsigned char *)((unsigned long)p_mem & 0xfffff000);

	


	StartCPU(1);
	
	return STATUS_SUCCESS
}

void StartCPU(int CPUID)
{

	unsigned int acpi_ICR1_temp=0;
	unsigned int acpi_ICR2_temp=0;

	LARGE_INTEGER startTime;
	LARGE_INTEGER currentTime;

	CMOS_WRITE(0x0f,0x0a);   //System Shutdown Code

	acpi_ICR2_temp=0x01000000;   //select cpu 1
	acpi_ICR1_temp=0x00004500;	//send init ipi
	*(unsigned long *)((unsigned char *)pAPIC + 0x310)=acpi_ICR2_temp;
	*(unsigned long *)((unsigned char *)pAPIC + 0x300)=acpi_ICR1_temp;

	KeQuerySystemTime(&startTime);
	while(1){
		KeQuerySystemTime(&currentTime);
		if((currentTime.QuadPart-startTime.QuadPart) >= 10*10000) break;
	}

	acpi_ICR2_temp=0x01000000;   //select cpu 1
	acpi_ICR1_temp=0x00004680;	//send start ipi   and  start from 0x00080000
	*(unsigned long *)((unsigned char *)pAPIC + 0x310)=acpi_ICR2_temp;
	*(unsigned long *)((unsigned char *)pAPIC + 0x300)=acpi_ICR1_temp;


	KeQuerySystemTime(&startTime);
	while(1){
		KeQuerySystemTime(&currentTime);
		if((currentTime.QuadPart-startTime.QuadPart) >= 2) break;
	}

	acpi_ICR2_temp=0x01000000;   //select cpu 1
	acpi_ICR1_temp=0x00004680;	//send start ipi   and  start from 0x00080000
	*(unsigned long *)((unsigned char *)pAPIC + 0x310)=acpi_ICR2_temp;
	*(unsigned long *)((unsigned char *)pAPIC + 0x300)=acpi_ICR1_temp;

	KeQuerySystemTime(&startTime);
	while(1){
		KeQuerySystemTime(&currentTime);
		if((currentTime.QuadPart-startTime.QuadPart) >= 10*10000) break;
	}
}


VOID UnloadGiraffeOS(__in PDRIVER_OBJECT DriverObject)
{

	unsigned int acpi_ICR1_temp;
	unsigned int acpi_ICR2_temp;

	PDEVICE_OBJECT deviceObject = DriverObject->DeviceObject;
	UNICODE_STRING uniWin32NameString;

	PAGED_CODE();


	//to stop the cpu 1 ,just send init ipi

	CMOS_WRITE(0x0f,0x0a);   //System Shutdown Code

	acpi_ICR2_temp=0x01000000;   //select cpu 1
	acpi_ICR1_temp=0x00004500;	//send init ipi
	*(unsigned long *)((unsigned char *)pAPIC + 0x310)=acpi_ICR2_temp;
	*(unsigned long *)((unsigned char *)pAPIC + 0x300)=acpi_ICR1_temp;

	MmUnmapIoSpace(pAPIC, sizeof(APIC));


	//
	// Create counted string version of our Win32 device name.
	//

	RtlInitUnicodeString( &uniWin32NameString, DOS_DEVICE_NAME );


	//
	// Delete the link from our device name to a name in the Win32 namespace.
	//

	IoDeleteSymbolicLink( &uniWin32NameString );

	if ( deviceObject != NULL )
	{
		IoDeleteDevice( deviceObject );
	}
	

}



NTSTATUS
SioctlCreateClose(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
    )
/*++

Routine Description:

    This routine is called by the I/O system when the SIOCTL is opened or
    closed.

    No action is performed other than completing the request successfully.

Arguments:

    DeviceObject - a pointer to the object that represents the device
    that I/O is to be done on.

    Irp - a pointer to the I/O Request Packet for this request.

Return Value:

    NT status code

--*/

{
    UNREFERENCED_PARAMETER(DeviceObject);

    PAGED_CODE();

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;

    IoCompleteRequest( Irp, IO_NO_INCREMENT );

    return STATUS_SUCCESS;
}


NTSTATUS
SioctlDeviceControl(
    PDEVICE_OBJECT DeviceObject,
    PIRP Irp
    )

/*++

Routine Description:

    This routine is called by the I/O system to perform a device I/O
    control function.

Arguments:

    DeviceObject - a pointer to the object that represents the device
        that I/O is to be done on.

    Irp - a pointer to the I/O Request Packet for this request.

Return Value:

    NT status code

--*/

{
    PIO_STACK_LOCATION  irpSp;// Pointer to current stack location
    NTSTATUS            ntStatus = STATUS_SUCCESS;// Assume success
    ULONG               inBufLength; // Input buffer length
    ULONG               outBufLength; // Output buffer length
    PCHAR               inBuf, outBuf; // pointer to Input and output buffer
    PCHAR               data = "This String is from Device Driver !!!";
    size_t              datalen = strlen(data)+1;//Length of data including null
    PMDL                mdl = NULL;
    PCHAR               buffer = NULL;

    UNREFERENCED_PARAMETER(DeviceObject);

    PAGED_CODE();

    irpSp = IoGetCurrentIrpStackLocation( Irp );
    inBufLength = irpSp->Parameters.DeviceIoControl.InputBufferLength;
    outBufLength = irpSp->Parameters.DeviceIoControl.OutputBufferLength;
	
    //
    // Determine which I/O control code was specified.
    //

    switch ( irpSp->Parameters.DeviceIoControl.IoControlCode )
    {
	case IOCTL_GIRAFFEOS_METHOD_BUFFERED:

        inBuf = Irp->AssociatedIrp.SystemBuffer;
        outBuf = Irp->AssociatedIrp.SystemBuffer;
		
		// This is a memory Test IOCTL, so just copy the in buffer to the out buffer.
		RtlCopyBytes(outBuf, inBuf, outBufLength);

        Irp->IoStatus.Information = outBufLength;
		      
       break;	

	case IOCTL_GIRAFFEOS_METHOD_APPLY_SETTING:
		
		if (!pBootSetting)
		{
			pBootSetting = ExAllocatePool(PagedPool, sizeof(BootSetting));
			RtlZeroBytes(pBootSetting, sizeof(BootSetting));
		}

		inBuf = Irp->AssociatedIrp.SystemBuffer;

		RtlCopyBytes(pBootSetting, inBuf, sizeof(BootSetting));

		break;

	case IOCTL_GIRAFFEOS_METHOD_SET_APPLICATION_BIN:

		
		break;

    default:

        //
        // The specified I/O control code is unrecognized by this driver.
        //

        ntStatus = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

End:
    //
    // Finish the I/O operation by simply completing the packet and returning
    // the same status as in the packet itself.
    //

    Irp->IoStatus.Status = ntStatus;

    IoCompleteRequest( Irp, IO_NO_INCREMENT );

    return ntStatus;
}
