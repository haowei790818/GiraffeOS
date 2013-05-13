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
#include "boot.h"


DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD UnloadGiraffeOS;

APIC * pAPIC=0;

NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject,__in PUNICODE_STRING RegistryPath)
{

	unsigned long has_local_apic=0;
	unsigned long pa=0;
	PHYSICAL_ADDRESS PhysicalAddress;
	unsigned char * p_mem=0;

	unsigned int acpi_ICR1_temp=0;
	unsigned int acpi_ICR2_temp=0;

	LARGE_INTEGER startTime;
	LARGE_INTEGER currentTime;

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
		return 0;

	pa = 0xFEE00000;
	__asm {
		mov ecx, 0x1B; // offset into apicbase
		rdmsr; //Load MSR specified by ECX into EDX:EAX
		mov edx, eax;
		and edx, 0xFFFFF000; // bits 31:12 contain the APICBASE address for the local apic
		mov pa, edx;
	}


	memset(&PhysicalAddress,0,sizeof(PHYSICAL_ADDRESS));
	PhysicalAddress.LowPart = (unsigned long)pa;
	pAPIC= (APIC *)MmMapIoSpace(PhysicalAddress, sizeof(APIC), MmNonCached);



	memset(&PhysicalAddress,0,sizeof(PHYSICAL_ADDRESS));
	PhysicalAddress.LowPart = (unsigned long)0x00080000;
	p_mem= (unsigned char  *)MmMapIoSpace(PhysicalAddress, sizeof(ap_boot_code), MmNonCached);
	memcpy(p_mem,&ap_boot_code,sizeof(ap_boot_code));
	MmUnmapIoSpace(p_mem, sizeof(ap_boot_code));



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



    return 0;
}


VOID UnloadGiraffeOS(__in PDRIVER_OBJECT DriverObject)
{

	unsigned int acpi_ICR1_temp;
	unsigned int acpi_ICR2_temp;

	//to stop the cpu 1 ,just send init ipi

	CMOS_WRITE(0x0f,0x0a);   //System Shutdown Code

	acpi_ICR2_temp=0x01000000;   //select cpu 1
	acpi_ICR1_temp=0x00004500;	//send init ipi
	*(unsigned long *)((unsigned char *)pAPIC + 0x310)=acpi_ICR2_temp;
	*(unsigned long *)((unsigned char *)pAPIC + 0x300)=acpi_ICR1_temp;

	MmUnmapIoSpace(pAPIC, sizeof(APIC));

}
