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
	this->variable = ((double)rand() / (RAND_MAX))*100;
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
	this->health +=10;
}

void Player::decreaseAmmo()
{
	this->ammo -= 2;
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

void Player::simulateShoot(int maze[MSZ][MSZ], double** securityMap)
{
	for (int i = 0; i < 360; i = i + 20)
	{
		bullet->SetX(x);
		bullet->SetY(y);
		bullet->SetDirX(cos(i));
		bullet->SetDirY(sin(i));
		bullet->Shoot();
		bool stop = true;
		int row, col;
		double delta = 10;

		while (stop)
		{
			stop = false;

			if (bullet->GetIsMoving())
			{
				
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

void Player::shoot(Target t, int maze[MSZ][MSZ],double securityMap[MSZ][MSZ], std::set<Point2D*>* bullets)
{
	if (this->ammo > 0)
	{	
		this->decreaseAmmo();
		bullet->SetX(x);
		bullet->SetY(y);
		double dx = t.getX() - x;
		double dy = t.getY() - y;
		double dis = sqrt(pow(dx, 2) + pow(dy, 2));//=sqrt(dx^2 +dy^2) TODO
		double dirX = dx / (dis / SPEED);
		double dirY = dy / (dis / SPEED);
		int currentdisX = dx > 0 ? (int)(dis / SPEED) / (int)dx : 0;
		int currentDisY = dy > 0 ? (int)(dis / SPEED) / (int)dy : 0;
		int doffX = 0, diffY = 0;
		bullet->SetDirX(dirX);
		bullet->SetDirY(dirY);
		bullet->Shoot();
		bool stop = false;
		bullet->Move(maze);
		int row, col;
		double lastX, lastY, currentX, currentY;
		double power = 50;
		while (!stop)
		{
			stop = true;

			if (bullet->GetIsMoving())
			{

				row = bullet->GetY();
				col = bullet->GetX();
				if (bullets)
				{
					Point2D* p = (Point2D*)malloc(sizeof(Point2D));
					p->setX(col);
					p->setY(row);
					bullets->insert(p);
				}
					
				if (row >= 0 && row < MSZ && col >= 0 && col < MSZ)
				{
					if (maze[row][col] == SPACE)
					{
						securityMap[row][col] += power;
						bullet->Move(maze);
						power = power - 0.01;//need to be checked
						stop = false;
						
						

					}

					if (maze[row][col] == otherPlayer)
					{

						Player* opponentPlater = nullptr;
						for (auto it = opponentsTeam.begin(); it != opponentsTeam.end(); ++it)
						{
							opponentPlater = *it;
							if (*opponentPlater == t)
								break;
						}


						opponentPlater->decreaseHealth(20 / dis);
						stop = true;
					}
					if (row == y && col == x)
					{
						stop = false;
						bullet->Move(maze);
					}
					if (stop)
						currentdisX = currentdisX;
					diffY++;
					doffX++;
				}

			}

		}

	}
	//if grnade is in range activate grnade 
	
}
std::list<Player*> Player::getOpponnentsTeam() {
	return opponentsTeam;
}
void Player::setOpponentsTeam(std::list<Player*> opponentsTeam)
{
	this->opponentsTeam = opponentsTeam;
}

Bullet* Player::getBullet()
{
	return bullet;
}
