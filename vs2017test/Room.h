#ifndef __ROOM_H
#define __ROOM_H

#pragma once
#include "Room.h"
#include <math.h>
#include "Const.h"

class Room
{
private:
	int cx, cy, width, height;
public:
	Room();
	Room(int row, int col, int w, int h);
	void SetCenterX(int col);
	void SetCenterY(int row);
	void SetWidth(int w);
	void SetHeight(int h);

	int GetCenterX();
	int GetCenterY();
	int GetWidth();
	int GetHeight();

	void Init(int m[MSZ][MSZ]);
	bool Overlap(int w, int h, int cx, int cy);


};

#endif // __ROOM_H