#include "Room.h"

Room::Room()
{
	cy = MSZ/2;
	cx = MSZ / 2;
	width = MSZ / 2;
	height = MSZ / 2;

}

Room::Room(int row, int col, int w, int h)
{
	cy = row;
	cx = col;
	width = w;
	height = h;
}

void Room::SetCenterX(int col)
{
	cx = col;
}

void Room::SetCenterY(int row)
{
	cy = row;
}

void Room::SetWidth(int w)
{
	width = w;
}

void Room::SetHeight(int h)
{
	height = h;
}

void Room::SetObjects(std::vector<Point2D> o)
{
	objects = o;
}

int Room::GetCenterX()
{
	return cx;
}

int Room::GetCenterY()
{
	return cy;
}

int Room::GetWidth()
{
	return width;
}

int Room::GetHeight()
{
	return height;
}

std::vector<Point2D> Room::GetObjects()
{
	return this->objects;
}

void Room::Init(int m[MSZ][MSZ])
{
	int i, j;

	for (i = cy - height / 2; i <= cy + height / 2; i++)
		for (j = cx - width / 2; j <= cx + width / 2; j++)
			m[i][j] = SPACE;
}

bool Room::Overlap(int w, int h, int cx, int cy)
{
	int hdist, vdist; // distance between center of rooms

	hdist = abs(cx - this->cx);
	vdist = abs(cy - this->cy);

	// if both horizontal distance between two rooms centers is below sum of these rooms half widths
	// and vertical distance between two rooms centers is below sum of these rooms half heights
	// then they overlap. otherwise they do not.
	return this->width/2+w/2>hdist-4 && vdist-4<this->height/2+h/2;
}
