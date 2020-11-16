#ifndef __PLAYER_H
#define __PLAYER_H
#pragma once

#include "AStar.h"
#include <thread>
#include "Target.h"
#include "Bullet.h"
#include "Bullet.h"
#include <list>
class Player: public Target
{
public:
	Player(Point2D* pos,int targets,int otherPlayer);
	void decreaseHealth();
	void decreaseHealth(int gap);
	void refillHealth();
	void decreaseAmmo();
	void refillAmmo();
	int getHealth();
	int getAmmo();
	void mouve(Point2D* next);
	void simulateShoot(int maze[MSZ][MSZ],int securityMap[MSZ][MSZ]);
	void shoot(Target t, int maze[MSZ][MSZ]);
	void setOpponentsTeam(std::list<Player>opponentsTeam);
	/*~Player();
	void Player::setMonsters(int numberOfMonster, Point2D**& monsters);
	void Player::setCoins(int numberOfCoins, Point2D**& coins);
	void run();*/
private:
	//bool win;
	Bullet* bullet;
	int otherPlayer;
	int health;
	int ammo;
	std::vector<Player>opponentsTeam;
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
