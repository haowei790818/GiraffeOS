#pragma once

typedef struct _BootSetting BootSetting, *PBootSetting;

class CGiraffeDriver
{
public:
	CGiraffeDriver();
	~CGiraffeDriver();

	BOOL StartDriver();
	void StopDriver();

	void ApplySettingToGiraffe(BootSetting * pBootSetting);
	void SetApplicationBin(unsigned char * data, unsigned long length);
	void StartApplication();
	void StopApplication();	

	BOOL IsApplicationRunning();



};

