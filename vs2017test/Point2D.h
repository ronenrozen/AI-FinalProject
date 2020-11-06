#ifndef __POINT2D_H
#define __POINT2D_H

#pragma once
class Point2D
{
public:
	Point2D();
	~Point2D();
	Point2D(int x, int y);
	int getX();
	int getY();
	bool operator==(const Point2D& other);
	void copy(const Point2D& other);
private:
	int x;
	int y;
};
#endif //__POINT2D_H
