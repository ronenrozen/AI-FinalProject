#include "Storage.h"

Storage::Storage(Point2D* pos)
{
	this->pos = pos;
	this->capacity = 100;
}

void Storage::refillHealth(Player p)
{
	p.refillHealth();
}

void Storage::refillAmmo(Player p)
{
	p.refillAmmo();
}

void Storage::decreaseHealth(Player p)
{
	p.decreaseHealth();
}

void Storage::decreaseAmmo(Player p)
{
	p.decreaseAmmo();
}


