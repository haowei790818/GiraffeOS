#pragma once
class CGiraffeTaskCtrl
{
private:
	bool runState;

public:
	CGiraffeTaskCtrl(void);
	~CGiraffeTaskCtrl(void);

	bool get_TaskRunState();
	void set_TaskRunState(bool state);

};

