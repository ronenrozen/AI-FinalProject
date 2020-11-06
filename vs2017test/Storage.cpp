#include "Storage.h"

Storage::Storage(Point2D* pos)
{
	this->pos = pos;
	this->capacity = 100;
}

void Storage::refill()
{
	this->capacity = 100;
}

void Storage::decrease()
{
	this->capacity--;
}
