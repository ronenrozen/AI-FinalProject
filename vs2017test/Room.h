#ifndef __ROOM_H
#define __ROOM_H

#pragma once
#include "Room.h"
#include <math.h>
#include "Const.h"
#include <vector>
#include "Point2D.h"

class Room
{
private:
	int cx, cy, width, height;
	std::vector<Point2D> objects;
public:
	Room();
	Room(int row, int col, int w, int h);
	void SetCenterX(int col);
	void SetCenterY(int row);
	void SetWidth(int w);
	void SetHeight(int h);
	void SetObjects(std::vector<Point2D> o);

	int GetCenterX();
	int GetCenterY();
	int GetWidth();
	int GetHeight();
	std::vector<Point2D> GetObjects();

	void Init(int m[MSZ][MSZ]);
	bool Overlap(int w, int h, int cx, int cy);


};

#endif // __ROOM_H