#ifndef __PARAMETERSOURCE_H__
#define __PARAMETERSOURCE_H__

#include "Parameter.h"

class ParameterSource
{
public:
	virtual void onParameterUpdated(const Parameter &parameter) = 0;
};

#endif //__PARAMETERSOURCE_H__