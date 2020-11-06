#ifndef __STORAGE_H
#define __STORAGE_H

#pragma once
#include "Point2D.h"
#include "Player.h"
class Storage
{
public:
	Storage(Point2D* pos);
	void refillHealth(Player p);
	void refillAmmo(Player p);
	void decreaseHealth(Player p);
	void decreaseAmmo(Player p);

	/*~Player();
	void Player::setMonsters(int numberOfMonster, Point2D**& monsters);
	void Player::setCoins(int numberOfCoins, Point2D**& coins);
	void run();*/
private:
	Point2D* pos;
	int capacity;
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
#endif //__STORAGE_H
