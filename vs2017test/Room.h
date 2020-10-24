#pragma once

const int MSZ = 100;
const int WALL = 1;
const int SPACE = 2;
const int START = 3;
const int PATH = 5;
const int GRAY = 6;
const int BLACK_SPACE = 7;
const int TARGET = 8;


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

