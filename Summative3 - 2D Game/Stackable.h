#pragma once

#include <stack>

#include "CItem.h"

class Stackable
{
public:
	std::stack<CItem*> disabledStack;
};

