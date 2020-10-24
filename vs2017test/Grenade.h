#pragma once
#include "Bullet.h"
const int NUM_BULLETS = 20;
const double PI = 3.14;

class Grenade
{
private:
	Bullet bullets[NUM_BULLETS];
	double  x, y;
public:
	Grenade(double x, double y);
	void Explode();
	void Move(int maze[MSZ][MSZ]);
	void Draw();
	void UpdateSecurityMap(int maze[MSZ][MSZ], double map[MSZ][MSZ]);


};

