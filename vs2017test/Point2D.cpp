#include "Point2D.h"


Point2D::Point2D()
{
}


Point2D::~Point2D()
{
}

Point2D::Point2D(int x, int y)
{
	this->x = x;
	this->y = y;
}


int Point2D::getX()
{
	return x;
}


int Point2D::getY()
{
	return y;
}


bool Point2D::operator==(const Point2D& other)
{
	return x == other.x && y == other.y;
}

void Point2D::copy(const Point2D& other)
{
	this->x = other.x;
	this->y = other.y;
}