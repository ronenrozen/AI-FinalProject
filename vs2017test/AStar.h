#ifndef __ASTAR_H
#define __ASTAR_H

#pragma once

#include "Const.h"
#include "Point2D.h"
#include "Point2D_hg.h"
#include "ComparePoints.h"
#include <vector>
#include <queue>

class AStar
{
public:
	AStar(Point2D*& pos);
	~AStar();
	bool run(Point2D targetPoint, int securityMap[MSZ][MSZ],int maxG = -1);
	Point2D* getTop();
	int getLen();
private:
	std::vector<Point2D_hg> solution;
	Point2D* last;
	Point2D* pos;
	Point2D lastPos;
	int getColor(Point2D& point);
};
#endif __ASTAR_H