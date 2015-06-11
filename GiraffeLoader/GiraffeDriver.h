#pragma once
class CGiraffeDriver
{
public:
	CGiraffeDriver();
	~CGiraffeDriver();

	BOOL StartDriver();
	void StopDriver();

	void ApplySettingToGiraffe(unsigned long cpuID, unsigned long memorySize);
	void SetApplicationBin(unsigned char * data, unsigned long length);
	void StartApplication();
	void StopApplication();	

	BOOL IsApplicationRunning();



};

