#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterID.h"
#include "Utilities.h"

// Parameter base class
class Parameter
{
public:
	Parameter(ParameterID id, String name)
		: id(id), name(name)
	{
	}

	ParameterID getParameterID(void) const { return id; }
	const String& getName(void) const { return name; }
	
	virtual var getValue(void) const { return 0; }
	virtual void setValue(var value) {}
	virtual String getDisplayValue(void) const { return ""; }
	virtual var resetValue(void) { return 0; }
	virtual float getNormalizedValue(void) const { return 0; }
	virtual void setNormalizedValue(float value) { (void)value; }

protected:
	ParameterID id;
	String name;
};

// Floating point parameter
class FloatParameter
	: public Parameter
{
public:
	FloatParameter(ParameterID id, String name, float defaultValue, float minValue, float maxValue)
		: Parameter(id, name),
		  defaultValue(defaultValue), 
		  currentValue(defaultValue), 
		  minValue(minValue),
		  maxValue(maxValue)
	{
	}

	virtual var getValue(void) const { return currentValue; }
	virtual void setValue(var value) { currentValue = value; }
	virtual float getDefaultValue(void) const { return defaultValue; }
	virtual float getMinValue(void) const { return minValue; }
	virtual float getMaxValue(void) const { return maxValue; }
	virtual String getDisplayValue(void) const { return String(currentValue, 2); }
	virtual var resetValue(void) { currentValue = defaultValue; return currentValue; }
	virtual float getNormalizedValue(void) const { return normalize(currentValue, minValue, maxValue); }
	virtual void setNormalizedValue(float value) { currentValue = scale(value, minValue, maxValue); }

protected:
	float defaultValue;
	float currentValue;
	float minValue;
	float maxValue;
};

// Integer parameter
class IntParameter
	: public Parameter
{
public:
	IntParameter(ParameterID id, String name, int defaultValue, int minValue, int maxValue)
		: Parameter(id, name),
		  defaultValue(defaultValue), 
		  currentValue(defaultValue), 
		  minValue(minValue),
		  maxValue(maxValue)
	{
	}

	virtual var getValue(void) const { return currentValue; }
	virtual void setValue(var value) { currentValue = value; }
	virtual int getDefaultValue(void) const { return defaultValue; }
	virtual int getMinValue(void) const { return minValue; }
	virtual int getMaxValue(void) const { return maxValue; }
	virtual String getDisplayValue(void) const { return String((float)currentValue, 2); }
	virtual var resetValue(void) { currentValue = defaultValue; return currentValue; }
	virtual float getNormalizedValue(void) const { return normalize((float)currentValue, (float)minValue, (float)maxValue); }
	virtual void setNormalizedValue(float value) { currentValue = (int)floor(scale((float)value, (float)minValue, (float)maxValue)); }

protected:
	int defaultValue;
	int currentValue;
	int minValue;
	int maxValue;
};

// String parameter
class StringParameter
		: public Parameter
{
public:
	StringParameter(ParameterID id, String name, String defaultValue)
		: Parameter(id, name),
		  defaultValue(defaultValue), 
		  currentValue(defaultValue)
	{
	}

	virtual var getValue(void) const { return currentValue; }
	virtual void setValue(var value) { currentValue = value; }
	virtual String getDefaultValue(void) const { return defaultValue; }
	virtual String getDisplayValue(void) const { return currentValue; }
	virtual var resetValue(void) { currentValue = defaultValue; return currentValue; }

protected:
	String defaultValue;
	String currentValue;
};

// Array parameter
class ArrayParameter
		: public Parameter
{
public:
	ArrayParameter(ParameterID id, String name, Array<var> defaultValue)
		: Parameter(id, name)
	{
		this->defaultValue.addArray(defaultValue, defaultValue.size());
		this->currentValue.addArray(defaultValue, defaultValue.size());
	}

	virtual var getValue(void) const { return currentValue; }
	virtual String getDisplayValue(void) const { return ""; }

	virtual void setValue(var value) {
		if (value.isArray()) {
			Array<var> *arr = value.getArray();
			currentValue.clear();
			currentValue.addArray(*arr, arr->size());
		}
	}

	virtual var resetValue(void) { 
		currentValue.clear();
		currentValue.addArray(defaultValue, defaultValue.size());
		return currentValue;
	}

protected:
	Array<var> defaultValue;
	Array<var> currentValue;
};

// ReferenceCountedObject parameter
class ReferenceCountedObjectParameter
	: public Parameter
{
	public:
	ReferenceCountedObjectParameter(ParameterID id, String name, ReferenceCountedObject *value)
		: Parameter(id, name)
	{
		obj = value;
	}

	virtual var getValue(void) const { return obj; }
	virtual String getDisplayValue(void) const { return ""; }

	virtual void setValue(var value) {
		if (value.isObject()) {
			obj = value;
		}
	}

	virtual var resetValue(void) {
		// can't reset
		return obj;
	}

protected:
	var obj;
};

#endif //__PARAMETER_H__