#include "Player.h"
#include "Const.h"
extern int maze[MSZ][MSZ];
extern bool done;

Player::Player(Point2D* pos, int target, int otherPlayer) :Target(pos->getX(), pos->getY(), target)
{

	bullet = new Bullet(double(x), double(y), false, 0.1, 0.1);
	this->health = 100;
	this->ammo = 100;
	this->otherPlayer = otherPlayer;

}

void Player::decreaseHealth()
{
	this->health--;
}

void Player::decreaseHealth(int gap)
{
	health = health - gap;
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

int Player::getHealth()
{
	return health;
}

int Player::getVariable()
{
	return variable;
}

int Player::getAmmo()
{
	return ammo;
}

void Player::mouve(Point2D* next)
{
	this->setX( next->getX());
	this->setY(next->getY());
}

void Player::simulateShoot(int maze[MSZ][MSZ], double securityMap[MSZ][MSZ])
{
	for (int i = 0; i < 360; i = i + 20)
	{
		bullet->SetX(x);
		bullet->SetY(y);
		bullet->SetDirX(cos(i));
		bullet->SetDirY(sin(i));
		bullet->Shoot();
		bool stop = false;
		int row, col;
		double delta = 10;

		while (!stop)
		{
			stop = true;

			if (bullet->GetIsMoving())
			{
				stop = false;
				row = MSZ * (bullet->GetY() + 1) / 2;
				col = MSZ * (bullet->GetX() + 1) / 2;
				if (row >= 0 && row < MSZ && col >= 0 && col < MSZ && maze[row][col] == SPACE && delta>0)
				{
					securityMap[row][col] += delta;
					bullet->Move(maze);
					stop = true;
				}

			}
			delta = delta - 0.1;//need to be checeked
		}
	}
}

void Player::shoot(Target t, int maze[MSZ][MSZ],double securityMap[MSZ][MSZ])
{
	//if grnade is in range activate grnade 
	bullet->SetX(x);
	bullet->SetY(y);
	double dx = t.getX() - x;
	double dy = t.getY() - y;
	double dis = sqrt(pow(dx, 2) + pow(dy, 2));//=sqrt(dx^2 +dy^2) TODO
	double dirX = x / dis;
	double dirY = y / dis;
	bullet->SetDirX(dirX);
	bullet->SetDirY(dirY);
	bullet->Shoot();
	bool stop = false;
	bullet->Move(maze);
	int row, col;
	int power = 50;
	while (!stop)
	{
		stop = true;

		if (bullet->GetIsMoving())
		{

			row = MSZ * (bullet->GetY() + 1) / 2;
			col = MSZ * (bullet->GetX() + 1) / 2;
			if (row >= 0 && row < MSZ && col >= 0 && col < MSZ)
			{
				if (maze[row][col] == SPACE)
				{
					bullet->Move(maze);
					power = power - 1;//need to be checked
					stop = false;
					securityMap[row][col] += 0.1;
				}

				if (maze[row][col] == otherPlayer)
				{

					Player* opponentPlater=nullptr;
					for (auto it = opponentsTeam.begin(); it != opponentsTeam.end(); ++it)
					{
						opponentPlater = *it;
						if (*opponentPlater == t)
							break;
					}
				

					opponentPlater->decreaseHealth(power);
					stop = false;
				}

			}

		}

	}
}
std::list<Player*> Player::getOpponnentsTeam() {
	return opponentsTeam;
}
void Player::setOpponentsTeam(std::list<Player*> opponentsTeam)
{
	this->opponentsTeam = opponentsTeam;
}