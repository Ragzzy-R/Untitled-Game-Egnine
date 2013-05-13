#pragma once

#include <sstream>
class Util
{
public:
	Util(void);
	template <class anyType>
	std::string toString(anyType val)
	{
		std::ostringstream convert;   
		convert<<val;      
		return convert.str();
	}
	~Util(void);
};

