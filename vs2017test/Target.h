#include "Point2D.h"
#pragma once
 class Target: public Point2D
{
public:
	Target(int x, int y,int target);
	int getTarget();
private:
	int target;
};

