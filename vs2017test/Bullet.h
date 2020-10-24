#pragma once
#include "Room.h"

const double SPEED = 0.01;

class Bullet
{
private:
	double x, y;
	double dirx, diry;
	bool isMoving;

public:
	Bullet();
	Bullet(double bx, double by, bool mv, double dx, double dy);
	void Move(int maze[MSZ][MSZ]);
	void Shoot();
	bool GetIsMoving();
	void Draw();
	void SetX(double x);
	void SetY(double y);
	void SetDirX(double dx);
	void SetDirY(double dy);
	double GetX();
	double GetY();
	void StopMoving();

};

