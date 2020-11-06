#ifndef __PLAYER_H
#define __PLAYER_H
#pragma once

#include "Point2D.h"
#include "AStar.h"
#include <thread>

class Player
{
public:
	Player(Point2D* pos);
	void decreaseHealth();
	void refillHealth();
	void decreaseAmmo();
	void refillAmmo();
	/*~Player();
	void Player::setMonsters(int numberOfMonster, Point2D**& monsters);
	void Player::setCoins(int numberOfCoins, Point2D**& coins);
	void run();*/
private:
	//bool win;

	Point2D* pos;
	int health;
	int ammo;
	/*vector<Point2D> coins;
	vector<AStar*> aStarCoins;
	Point2D* lastPosCoin;
	int indexCoin;

	vector<Point2D*> monsters;
	AStar* runMonster;
	Point2D* lastPosRunMonster;
	bool runFromMonster();
	void newFromMonsterPoint(Point2D*& monster);

	void searchCoins();*/
};
#endif // __PLAYER_H_
