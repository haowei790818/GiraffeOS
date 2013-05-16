#include "StdAfx.h"
#include "GiraffeTaskCtrl.h"


CGiraffeTaskCtrl::CGiraffeTaskCtrl(void)
{
	runState=false;
}


CGiraffeTaskCtrl::~CGiraffeTaskCtrl(void)
{
}

bool CGiraffeTaskCtrl::get_TaskRunState()
{
	return this->runState;
}

void CGiraffeTaskCtrl::set_TaskRunState( bool state )
{
	this->runState=state;
}
