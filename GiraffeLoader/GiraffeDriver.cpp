#include <Windows.h>
#include <boot.h>

#include "stdafx.h"
#include "global.h"
#include "GiraffeDriver.h"



CGiraffeDriver::CGiraffeDriver()
{
}


CGiraffeDriver::~CGiraffeDriver()
{
}


bool CGiraffeDriver::StartDriver()
{
	TCHAR sername[32] = GIRAFFEOS_SERVICE_NAME;
	TCHAR disname[32] = GIRAFFEOS_SERVICE_DISPLAY_NAME;
	TCHAR path[256] = GIRAFFEOS_DRIVER_PATH;
	SC_HANDLE scmHandle;
	SC_HANDLE serviceHandle;

	do
	{
		SC_HANDLE scmHandle = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (!scmHandle)
		{
			break;
		}
		SC_HANDLE serviceHandle = ::CreateService(scmHandle,
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
	
	return false;
}

void CGiraffeDriver::StopDriver()
{
	TCHAR sername[32] = GIRAFFEOS_SERVICE_NAME;
	SC_HANDLE scmHandle;
	SC_HANDLE serviceHandle;
	SERVICE_STATUS ss;

	scmHandle = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!scmHandle)
	{
		return;
	}

	serviceHandle = OpenService(scmHandle, sername, SERVICE_ALL_ACCESS);
	if (!serviceHandle)
	{
		return;
	}
	ControlService(serviceHandle, SERVICE_CONTROL_STOP, &ss);
	DeleteService(serviceHandle);

	if (!serviceHandle)
	{
		CloseServiceHandle(serviceHandle);
	}

	if (!scmHandle)
	{
		CloseServiceHandle(scmHandle);
	}
}

void CGiraffeDriver::ApplySettingToGiraffe(int cpuID, unsigned int memorySize)
{

}

void CGiraffeDriver::SetApplicationBin(unsigned char * data, unsigned int length)
{

}

void CGiraffeDriver::StartApplication()
{

}

void CGiraffeDriver::StopApplication()
{

}