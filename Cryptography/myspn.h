#pragma once
#include "stdafx.h"

class MySPN {
public:
	MySPN();
	void setKey(std::string key);
	void setKey(unsigned long long key);
private:
	unsigned long long thisKey;
	
};