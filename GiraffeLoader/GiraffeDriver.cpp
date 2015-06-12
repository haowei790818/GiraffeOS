#include "stdafx.h"
#include <Windows.h>
#include <winsvc.h>
#include <boot.h>
#include <winioctl.h>
#include <ioctl.h>
#include "global.h"
#include "GiraffeDriver.h"



CGiraffeDriver::CGiraffeDriver()
{
}


CGiraffeDriver::~CGiraffeDriver()
{
}


BOOL CGiraffeDriver::StartDriver()
{
	TCHAR sername[32] = GIRAFFEOS_SERVICE_NAME;
	TCHAR disname[32] = GIRAFFEOS_SERVICE_DISPLAY_NAME;
	TCHAR path[256] = GIRAFFEOS_DRIVER_PATH;
	SC_HANDLE scmHandle;
	SC_HANDLE serviceHandle;
	DWORD debugCode = 0;

	do
	{
		scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (!scmHandle)
		{
			debugCode = GetLastError();
			break;
		}
		serviceHandle = CreateService(scmHandle,
			sername,
			disname,
			SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
			SERVICE_DEMAND_START,
			SERVICE_ERROR_NORMAL,
			path,
			NULL, NULL, NULL, NULL, NULL);

		if (serviceHandle == NULL)
		{
			DWORD dwRtn = GetLastError();
			if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
			{
				break;
			}

			serviceHandle = OpenService(scmHandle, sername, SERVICE_ALL_ACCESS);
			if (serviceHandle == NULL)
			{
				break;
			}
		}

		return StartService(serviceHandle, NULL, NULL);
	} while (false);


	if (!serviceHandle)
	{
		CloseServiceHandle(serviceHandle);
	}

	if (!scmHandle)
	{
		CloseServiceHandle(scmHandle);
	}
	
	return FALSE;
}

void CGiraffeDriver::StopDriver()
{
	TCHAR sername[32] = GIRAFFEOS_SERVICE_NAME;
	SC_HANDLE scmHandle;
	SC_HANDLE serviceHandle;
	SERVICE_STATUS ss;
	do
	{
		scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (!scmHandle)
		{
			break;
		}

		serviceHandle = OpenService(scmHandle, sername, SERVICE_ALL_ACCESS);
		if (!serviceHandle)
		{
			break;
		}
		ControlService(serviceHandle, SERVICE_CONTROL_STOP, &ss);
		DeleteService(serviceHandle);

	} while (false);
	

	if (serviceHandle)
	{
		CloseServiceHandle(serviceHandle);
	}

	if (scmHandle)
	{
		CloseServiceHandle(scmHandle);
	}
}

void CGiraffeDriver::ApplySettingToGiraffe(BootSetting * pBootSetting)
{

	HANDLE device = CreateFile(NT_DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD retLen = 0;
	DeviceIoControl(device, IOCTL_GIRAFFEOS_METHOD_APPLY_SETTING, pBootSetting, sizeof(BootSetting), NULL, 0, &retLen, NULL);
	CloseHandle(device);
	
}

void CGiraffeDriver::SetApplicationBin(unsigned char * data, unsigned long length)
{
	HANDLE device = CreateFile(NT_DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD retLen = 0;
	DeviceIoControl(device, IOCTL_GIRAFFEOS_METHOD_SET_APPLICATION_BIN, data, length, NULL, 0, &retLen, NULL);
	CloseHandle(device);

}

void CGiraffeDriver::StartApplication()
{
	HANDLE device = CreateFile(NT_DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD retLen = 0;
	DeviceIoControl(device, IOCTL_GIRAFFEOS_METHOD_START_APPLICATION, NULL, 0, NULL, 0, &retLen, NULL);
	CloseHandle(device);
}

void CGiraffeDriver::StopApplication()
{
	HANDLE device = CreateFile(NT_DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD retLen = 0;
	DeviceIoControl(device, IOCTL_GIRAFFEOS_METHOD_STOP_APPLICATION, NULL, 0, NULL, 0, &retLen, NULL);
	CloseHandle(device);
}

BOOL CGiraffeDriver::IsApplicationRunning()
{
	//HANDLE device = CreateFile(NT_DEVICE_NAME, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	//DWORD retLen = 0;
	//DeviceIoControl(device, IOCTL_GIRAFFEOS_METHOD_STOP_APPLICATION, NULL, 0, NULL, 0, &retLen, NULL);
	//CloseHandle(device);
	return FALSE;
}