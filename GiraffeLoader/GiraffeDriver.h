#pragma once
class CGiraffeDriver
{
public:
	CGiraffeDriver();
	~CGiraffeDriver();

	bool StartDriver();
	void StopDriver();

	void ApplySettingToGiraffe(int cpuID, unsigned int memorySize);
	void SetApplicationBin(unsigned char * data, unsigned int length);
	void StartApplication();
	void StopApplication();	

};

