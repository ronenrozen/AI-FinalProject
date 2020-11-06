#include "Player.h"

extern int maze[MSZ][MSZ];
extern bool done;

Player::Player(Point2D* pos)
{
	this->pos = pos;
	this->health = 100;
	this->ammo = 100;

}

void Player::decreaseHealth()
{
	this->health--;
}

void Player::refillHealth()
{
	this->health = 100;
}

void Player::decreaseAmmo()
{
	this->ammo--;
}

void Player::refillAmmo()
{
	this->ammo = 100;
}


//Player::~Player()
//{
//	coins.clear();
//	for (int i = 0; i < (int)aStarCoins.size(); i++)
//		delete aStarCoins[i];
//	aStarCoins.clear();
//	delete lastPosCoin;
//	delete pos;
//	delete runMonster;
//	delete lastPosRunMonster;
//}
//
//void Player::setMonsters(int numberOfMonster, Point2D**& monsters)
//{
//	AStar astar = AStar(pos);
//	for (int i = 0; i < numberOfMonster; i++)
//	{
//		if (astar.run(*(monsters[i])))
//			this->monsters.push_back(monsters[i]);
//	}
//}
//
//void Player::setCoins(int numberOfCoins, Point2D**& coins)
//{
//	AStar* astar;
//	for (int i = 0; i < numberOfCoins; i++)
//	{
//		astar = new AStar(pos);
//		if (astar->run(*(coins[i]))) {
//			this->coins.push_back(*(coins[i]));
//			this->aStarCoins.push_back(astar);
//		}
//		else
//			delete astar;
//	}
//	if (this->coins.size() == 0)
//		done = true;
//}
//
//void Player::run() {
//	int minH = MIN_RUN, temp, monsterIndex, numberOfUse = 0;
//	while (!done)
//	{
//		for (int i = 0; i < (int)monsters.size(); i++)
//		{
//			temp = abs(pos->getX() - monsters.at(i)->getX()) + abs(pos->getY() - monsters.at(i)->getY());// Manhattan Distance
//			if (i == 0 || minH > temp)
//			{
//				minH = temp;
//				monsterIndex = i;
//			}
//		}
//		if (minH < MIN_RUN)
//		{
//			if (numberOfUse <= 0 || lastPosRunMonster == NULL || !(*pos == *lastPosRunMonster))
//			{
//				if (lastPosRunMonster != NULL)
//				{
//					delete lastPosRunMonster;
//					lastPosRunMonster = NULL;
//				}
//				numberOfUse = USE_TIME;
//				newFromMonsterPoint(monsters.at(monsterIndex));
//			}
//			if (runFromMonster())
//				numberOfUse--;
//			else
//				numberOfUse = 0;
//			lastPosRunMonster = new Point2D(*pos);
//		}
//		else
//		{
//			searchCoins();
//		}
//		(maze)[pos->getY()][pos->getX()] = Player;
//	}
//	if (win)
//		maze[pos->getY()][pos->getX()] = Player_WIN;
//	else
//		maze[pos->getY()][pos->getX()] = Player_DIE;
//}
//
//bool Player::runFromMonster()
//{
//	Point2D* helper = runMonster->getTop();
//
//	if (helper != NULL)
//	{
//		this_thread::sleep_for(chrono::microseconds(Player_SLEEP));
//		(maze)[pos->getY()][pos->getX()] = SPACE;
//		pos->copy(*helper);
//		delete helper;
//		return true;
//	}
//	return false;
//}
//
//void Player::newFromMonsterPoint(Point2D*& monster)
//{
//	int mX, mY;
//	mX = monster->getX();
//	mY = monster->getY();
//	int posX, posY;
//	posX = pos->getX() - mX;
//	if (posX != 0)
//		posX = posX / abs(posX);
//	posY = pos->getY() - mY;
//	if (posY != 0)
//		posY = posY / abs(posY);
//	Point2D pos = Point2D((MSZ - mX + posX * 10) % MSZ, (MSZ - mY + posY * 10) % MSZ);
//	runMonster->run(pos, RUN);
//}
//
//void Player::searchCoins()
//{
//	if (lastPosCoin == NULL || !(*pos == *lastPosCoin))
//	{
//		int minLen, temp;
//		for (int i = 0; i < (int)coins.size(); i++)
//		{
//			aStarCoins[i]->run(coins[i]);
//			temp = aStarCoins[i]->getLen();
//			if (i == 0 || minLen > temp) {
//				indexCoin = i;
//				minLen = temp;
//			}
//		}
//	}
//	Point2D* helper = aStarCoins[indexCoin]->getTop();
//	this_thread::sleep_for(chrono::microseconds(Player_SLEEP));
//	(maze)[pos->getY()][pos->getX()] = SPACE;
//	pos->copy(*helper);
//	delete helper;
//	delete lastPosCoin;
//	if ((maze)[pos->getY()][pos->getX()] == COIN) {
//		coins.erase(coins.begin() + indexCoin);
//		delete aStarCoins[indexCoin];
//		lastPosCoin = NULL;
//		aStarCoins.erase(aStarCoins.begin() + indexCoin);
//		if (coins.size() == 0) {
//			done = true;
//			this->win = true;
//		}
//	}
//	else
//		lastPosCoin = new Point2D(*pos);
//}