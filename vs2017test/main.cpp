#include "glut.h"
#include <time.h>
#include <vector>
#include <iostream>
#include "Const.h"
#include "Node.h"
#include "Room.h"
#include <queue>
#include "CompareNodes.h"
#include "Bullet.h"
#include <math.h>
#include "Grenade.h"
#include <list>
#include <set>
#include "Point2D.h"
#include "Player.h"
#include "Storage.h"
using namespace std;


int maze[MSZ][MSZ];
int roomMat[MSZ][MSZ];
double security_map[MSZ][MSZ] = { 0 };

Room rooms[NUM_ROOMS]; // runs default constructor
std::list<Player> groupA;
std::list<Player> groupB;
std::vector<Target> ammoStorages;
std::vector<Target> healthStorages;
bool start_BFS = false;


Bullet* pb = nullptr;
Grenade* pg = nullptr;
Point2D** storages;

void SetupMaze();
void initTwoPlayers();
void initStorages();
void initObjects();
void init()
{
	//    RED, GREEN, BLUE
	glClearColor(0.8, 0, 0, 0);// color of window background

	srand(time(0));
	// define main axes
	glOrtho(-1, 1, -1, 1, -1, 1);

	SetupMaze();

	initTwoPlayers();
	initStorages();
	initObjects();
}

void UpdateSecurityMap(std::list<Player> B)
{
	security_map[MSZ][MSZ] = { 0 };
	for (std::vector<Player>::iterator it = B.begin(); it != B.end(); ++it)
	{
		it->simulateShoot(security_map);
	}
	//to do
	//int num_grenades = 5000;
	//int i;
	//double x, y;

	//for (i = 0; i < num_grenades; i++)
	//{
	//	//		Grenade* p = new Grenade(rand() % MSZ, rand() % MSZ); // here was the bug!!!
	//	x = 2 * (rand() % MSZ) / (double)MSZ - 1;
	//	y = 2 * (rand() % MSZ) / (double)MSZ - 1;

	//	Grenade* p = new Grenade(x, y);

	//	p->UpdateSecurityMap(maze, security_map);

	//	//		delete p;
	//}

}

double Distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
}

void UpdatePQ(priority_queue <Node*, vector<Node*>, CompareNodes>& pq, Node* pn)
{
	bool found = false;
	Node* ptmp;
	vector <Node*> tmp_v;

	while (!pq.empty() && !found)
	{
		ptmp = pq.top();
		pq.pop();
		if ((*ptmp).operator==(*pn))
		{
			found = true;
			pq.push(pn);
		}
		else
			tmp_v.push_back(ptmp);
	}

	while (!tmp_v.empty())
	{
		ptmp = tmp_v.back();
		tmp_v.pop_back();
		pq.push(ptmp);
	}
}

void 	AddNeighbor(Node* pCurrent, int direction,
	priority_queue <Node*, vector<Node*>, CompareNodes>& pq, vector <Node>& gray, vector <Node>& black,
	int target_index)
{
	int i, j;
	Node* pn;
	double g, h;
	vector<Node>::iterator it_gray;
	vector<Node>::iterator it_black;

	switch (direction)
	{
	case TOP:
		i = pCurrent->GetRow() + 1;
		j = pCurrent->GetCol();
		break;
	case BOTTOM:
		i = pCurrent->GetRow() - 1;
		j = pCurrent->GetCol();
		break;
	case LEFT:
		i = pCurrent->GetRow();
		j = pCurrent->GetCol() - 1;
		break;
	case RIGHT:
		i = pCurrent->GetRow();
		j = pCurrent->GetCol() + 1;
		break;
	}
	h = Distance(j, i, rooms[target_index].GetCenterX(), rooms[target_index].GetCenterY());
	if (maze[i][j] == WALL)
		g = pCurrent->GetG() + WALL_COST;
	else // SPACE or TARGET
		g = pCurrent->GetG() + SPACE_COST;
	/*	n = current; // this is the neighbor of pCurrent
		n.SetCol(j);
		n.SetRow(i);
		n.SetG(g);
		n.SetH(h);
		n.ComputeF();
		n.SetParent(&current);
		*/
	pn = new Node(i, j, pCurrent, g, h);
	// Let's check the color of neighbor
	it_black = find(black.begin(), black.end(), *pn); // Now we see that we really need operator ==
	if (it_black != black.end()) // it is black => we don't add it to pq 
	{
		delete pn;
		return;
	}
	else // it is not black 
	{
		it_gray = find(gray.begin(), gray.end(), *pn);
		if (it_gray == gray.end()) // it is not gray => it is white so add it to pq and paint it gray
		{
			pq.push(pn);
			gray.push_back(*pn);
		}
		else // it is gray => check if its F is better (lower) then of its copy from pq
		{
			if (it_gray->GetF() < pn->GetF()) // pn is not better then its copy from pq
			{
				delete pn;
				return;
			}
			else // pn is BETTER then its copy from pq
			{
				UpdatePQ(pq, pn);
				gray.erase(it_gray);
				gray.push_back(*pn);
			}
		}
	}
}

void RestorePath(Node* pCurrent)
{
	bool door = true;
	Node* lastParrent = pCurrent;
	while (maze[pCurrent->GetRow()][pCurrent->GetCol()] != START)
	{
		if (maze[pCurrent->GetRow()][pCurrent->GetCol()] == WALL)
		{
			if (door)
			{

				roomMat[pCurrent->GetRow()][pCurrent->GetCol()] = -2;
				door = false;
			}
		}
		else {
			if (!door)
			{
				roomMat[lastParrent->GetRow()][lastParrent->GetCol()] = -2;
				door = true;
			}
		}
		maze[pCurrent->GetRow()][pCurrent->GetCol()] = SPACE;
		pCurrent = pCurrent->GetParent();
	}
	maze[pCurrent->GetRow()][pCurrent->GetCol()] = SPACE;

}


// runs A* for building a path from rooms[index1] to rooms[index2]
void DigPath(int index1, int index2)
{
	vector <Node> gray;
	vector <Node> black;
	priority_queue <Node*, vector<Node*>, CompareNodes> pq;
	vector<Node>::iterator it_gray;
	vector<Node>::iterator it_black;

	int is, js, it, jt;
	Node* pn;
	Node* pCurrent;
	is = rooms[index1].GetCenterY();
	js = rooms[index1].GetCenterX();
	it = rooms[index2].GetCenterY();
	jt = rooms[index2].GetCenterX();
	// 1. set center of rooms[index1] as START
	maze[is][js] = START;
	// 2. set center of rooms[index2] as TARGET
	maze[it][jt] = TARGET;
	// 3. add start to PQ and to Grays 
	pn = new Node(is, js, nullptr, 0, Distance(js, is, jt, it));
	pq.push(pn);
	gray.push_back(*pn);
	// 4. while PQ is not empty
	while (!pq.empty())
	{
		// 4.1 pick the "BEST"( F = G+H ) Node from PQ
		pCurrent = pq.top();
		pq.pop(); // and remove it from pq
		// 4.2 if it is TARGET, the best path has been found
		if (maze[pCurrent->GetRow()][pCurrent->GetCol()] == TARGET)
		{
			//			maze[current.GetRow()][current.GetCol()] = SPACE;
						// 4.2.1 Restore path by combining digging (through walls) and moving (through spaces)
			RestorePath(pCurrent);
			return; // 
		}
		else // pCurrent is not TARGET
		{
			// paint pCurrent black
			black.push_back(*pCurrent);
			// remove pCurrent from gray
			it_gray = find(gray.begin(), gray.end(), *pCurrent); // based on operator == if we compare objects
			if (it_gray != gray.end()) // if it was found
				gray.erase(it_gray);
			// 4.3 Add neighbors of pCurrent to PQ and return to 4.
				// check top neighbor
			if (pCurrent->GetRow() + 1 < MSZ)
				AddNeighbor(pCurrent, TOP, pq, gray, black, index2);
			// check bottom neighbor
			if (pCurrent->GetRow() - 1 >= 0)
				AddNeighbor(pCurrent, BOTTOM, pq, gray, black, index2);
			// check left neighbor
			if (pCurrent->GetCol() - 1 >= 0)
				AddNeighbor(pCurrent, LEFT, pq, gray, black, index2);
			// check right neighbor
			if (pCurrent->GetCol() + 1 < MSZ)
				AddNeighbor(pCurrent, RIGHT, pq, gray, black, index2);
		}// else
	}// while
	cout << "ERROR: Priority queue is empty. The target was missed\n";

}

void DigPathes()
{
	int i, j;

	for (i = 0; i < NUM_ROOMS; i++)
		for (j = i + 1; j < NUM_ROOMS; j++)
			DigPath(i, j);

}


void SetupMaze()
{
	int i, j, k;
	int countRoom;
	int w, h, cx, cy;
	// set borders
	int healthStorage = 0;
	int ammoStorage = 0;

	for (i = 0; i < MSZ; i++) {
		for (j = 0; j < MSZ; j++) {
			maze[i][j] = WALL;
			roomMat[i][j] = -1;
		}
	}


	for (countRoom = 0; countRoom < NUM_ROOMS; countRoom++)
	{
		bool hasOverlap;
		do
		{
			hasOverlap = false;

			// pick random room parameters
			w = 7 + rand() % (MSZ / 5);
			h = 7 + rand() % (MSZ / 5);
			cx = 1 + w / 2 + rand() % (MSZ - w - 2);
			cy = 1 + h / 2 + rand() % (MSZ - h - 2);

			// check overlapping
			for (k = 0; k < countRoom && !hasOverlap; k++)
				if (rooms[k].Overlap(w, h, cx, cy))
					hasOverlap = true;
		} while (hasOverlap);
		// setup room
		rooms[countRoom].SetWidth(w);
		rooms[countRoom].SetHeight(h);
		rooms[countRoom].SetCenterX(cx);
		rooms[countRoom].SetCenterY(cy);
		rooms[countRoom].Init(maze, roomMat);
	}

	// now dig the pathes between each room
	DigPathes();
}

void DrawMaze()
{
	int i, j;
	double dx, dy, x, y;

	dx = dy = 2.0 / MSZ; // 2.0 is the logical width and height of the window

	y = -1;
	for (i = 0; i < MSZ; i++, y += dy)
	{
		x = -1;
		for (j = 0; j < MSZ; j++, x += dx)
		{
			switch (maze[i][j]) {
			case WALL:
				glColor3d(0.2, 0.2, 0.2);
				break;
			case SPACE:
				// glColor3d(1, 1, 1);
				glColor3d(1 - security_map[i][j], 1 - security_map[i][j], 1 - security_map[i][j]);
				break;
			case START:
				glColor3d(0.2, 0.4, 1);
				break;
			case TARGET:
				glColor3d(1, 0, 0);
				break;
			case BLACK_SPACE:
				glColor3d(1, 1, 0);
				break;
			case GRAY:
				glColor3d(0, 0.8, 0);
				break;
			case PATH:
				glColor3d(1, 0.5, 1);
				break;
			case HEALTH_STORAGE:
				glColor3d(1, 0.5, 0);
				break;
			case AMMO_STORAGE:
				glColor3d(0, 0, 0);
				break;
			case PLAYER1:
				glColor3d(0, 0, 1);
				break;
			case PLAYER2:
				glColor3d(0, 1, 0);
				break;
			case OBJECT:
				glColor3d(0.4, 0, 0);
				break;


			}// switch
			// draw one square in maze
			glBegin(GL_POLYGON);
			glVertex2d(x, y);
			glVertex2d(x, y + dy);
			glVertex2d(x + dx, y + dy);
			glVertex2d(x + dx, y);
			glEnd();
		}
	}
}

void BuildPath(Node* pCurrent)
{
	while (maze[pCurrent->GetRow()][pCurrent->GetCol()] != START)
	{
		maze[pCurrent->GetRow()][pCurrent->GetCol()] = PATH;
		pCurrent = pCurrent->GetParent();
	}
}

/*
void BFSIteration()
{
	Node* pCurrent;
	Node* next;
	if (gray.empty()) // there is  no solution
	{
		cout << "there is  no solution\n";
		start_BFS = false;
	}
	else
	{
		// 1. extract first element from gray
		pCurrent = gray[0];
		gray.erase(gray.begin());
		// 2. paint pCurrent black
		if(maze[pCurrent->GetRow()][pCurrent->GetCol()]!=START)
			maze[pCurrent->GetRow()][pCurrent->GetCol()] = BLACK_SPACE;
		// 3. Add white neighbours of pCurrent to gray
		// check up
		if (maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == SPACE) // SPACE means that it is WHITE
		{
			maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] = GRAY;
			next = new Node(pCurrent->GetRow() + 1, pCurrent->GetCol(), pCurrent); // create it
			gray.push_back(next);
		}
		else if (maze[pCurrent->GetRow() + 1][pCurrent->GetCol()] == TARGET)
		{
			cout << "the solution has been found\n";
			start_BFS = false;
			BuildPath(pCurrent);
		}

		// check right
		if (maze[pCurrent->GetRow() ][pCurrent->GetCol()+ 1] == SPACE) // SPACE means that it is WHITE
		{
			maze[pCurrent->GetRow() ][pCurrent->GetCol()+ 1] = GRAY;
			next = new Node(pCurrent->GetRow(), pCurrent->GetCol() + 1, pCurrent); // create it
			gray.push_back(next);
		}
		else if (maze[pCurrent->GetRow()][pCurrent->GetCol() + 1] == TARGET)
		{
			cout << "the solution has been found\n";
			start_BFS = false;
			BuildPath(pCurrent);
		}

		// check down
		if (maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == SPACE) // SPACE means that it is WHITE
		{
			maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] = GRAY;
			next = new Node(pCurrent->GetRow() - 1, pCurrent->GetCol(), pCurrent); // create it
			gray.push_back(next);
		}
		else if (maze[pCurrent->GetRow() - 1][pCurrent->GetCol()] == TARGET)
		{
			cout << "the solution has been found\n";
			start_BFS = false;
			BuildPath(pCurrent);
		}

		// check left
		if (maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] == SPACE) // SPACE means that it is WHITE
		{
			maze[pCurrent->GetRow()][pCurrent->GetCol() - 1] = GRAY;
			next = new Node(pCurrent->GetRow(), pCurrent->GetCol() - 1, pCurrent); // create it
			gray.push_back(next);
		}
		else if (maze[pCurrent->GetRow() ][pCurrent->GetCol()-1] == TARGET)
		{
			cout << "the solution has been found\n";
			start_BFS = false;
			BuildPath(pCurrent);
		}
	}
}
*/

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	DrawMaze();
	if (pg != nullptr)
		pg->Draw();

	glutSwapBuffers(); // show all
}

void idle()
{
	//if (pb != nullptr && pb->GetIsMoving())
	//	pb->Move(maze);

	if (pg != nullptr)
		pg->Move(maze); // move all bullets of grenade

//	if (start_BFS)
//		BFSIteration();
	glutPostRedisplay(); // call to display again
}

void menu(int choice)
{
	if (choice == 1) // BFS
	{
		play();
		//pb->Shoot();
	}
	//else if (choice == 2) // DFS
	//{
	//	pg->Explode();
	//}
}

// x, y are in pixels
void mouse(int button, int state, int x, int y)
{
	double bx, by, dx, dy, alpha;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{

		bx = 2 * x / (double)W - 1; // now bx is between -1 and 1
		by = 2 * (H - y) / (double)H - 1;

		alpha = (rand() % 360) * PI / 180;
		dx = cos(alpha);
		dy = sin(alpha);

		// pb = new Bullet(bx, by, false, dx, dy);
		pg = new Grenade(bx, by);
	}
}

set<int> randomTwoRoomsNums() {
	std::set<int> roomsToStorage;
	do {
		int roomNum = rand() % NUM_ROOMS;
		roomsToStorage.insert(roomNum);
	} while (roomsToStorage.size() < 2);
	return roomsToStorage;

}

Point2D* getPoint(Room room) {
	bool isFound = false;
	Point2D* pos;
	do {
		int randX = (rand() % abs(room.GetWidth() / 2));
		int randY = (rand() % abs(room.GetHeight() / 2));
		pos = new Point2D(room.GetCenterX() - randX, room.GetCenterY() + randY);
		if (maze[pos->getY()][pos->getX()] == SPACE)
			isFound = true;
	} while (!isFound);
	return pos;
}

void initTwoPlayers() {
	int count = 11;
	std::set<int> randomTwoRooms = randomTwoRoomsNums();
	for (auto it = randomTwoRooms.begin(); it != randomTwoRooms.end(); it++, count++) {
		Room room = rooms[*it];
		for (int i = 0; i < MAX_PLAYERS_IN_GROUP; i++) {
			Point2D* playersPos = getPoint(room);
			Player* player = new Player(playersPos, count, count + (1 * ((i + 1) % 2) - 1 * (i % 2)));
			maze[playersPos->getY()][playersPos->getX()] = count;
			count == 11 ? groupA.push_back(*player) : groupB.push_back(*player);
		}
	}

}

void initStorages() {
	std::set<int> randomTwoRoomsForHealthStorage = randomTwoRoomsNums();

	for (auto it = randomTwoRoomsForHealthStorage.begin(); it != randomTwoRoomsForHealthStorage.end(); it++) {
		Room room = rooms[*it];
		Point2D* healthStoragePos = getPoint(room);
		Storage* healthStorage = new Storage(healthStoragePos, HEALTH_STORAGE);
		maze[healthStoragePos->getY()][healthStoragePos->getX()] = HEALTH_STORAGE;
		healthStorages.push_back(*healthStorage);
	}
	std::set<int> randomTwoRoomsForAmmoStorage = randomTwoRoomsNums();
	for (auto it = randomTwoRoomsForAmmoStorage.begin(); it != randomTwoRoomsForAmmoStorage.end(); it++) {

		Room room = rooms[*it];
		Point2D* ammoStoragePos = getPoint(room);
		Storage* ammoStorage = new Storage(ammoStoragePos, AMMO_STORAGE);
		maze[ammoStoragePos->getY()][ammoStoragePos->getX()] = AMMO_STORAGE;
		ammoStorages.push_back(*ammoStorage);
	}
}


void initObjects() {
	int count = 0;
	for (int countRoom = 0; countRoom < NUM_ROOMS; countRoom++) {
		std::vector<Point2D> objects;
		Room room = rooms[countRoom];
		do {
			Point2D* pos = getPoint(room);
			maze[pos->getY()][pos->getX()] = OBJECT;
			objects.push_back(*pos);
			count++;
		} while (count < MAX_OBJECTS);
		room.SetObjects(objects);
		count = 0;
	}
}
void play(std::list<Player>A, std::list<Player> B)
{
	Player p1 = A.front();
	p1.setOpponentsTeam(B);
	A.pop_back();
	UpdateSecurityMap(B);
	Target target = chooseTarget(p1, B);
	Point2D* nextStep = Astar(&Point2D(p1.getX(), p1.getY()), Point2D(target.getX(), target.getY()), -1);
	int currentRoom = roomMat[p1.getX][p1.getY];
	if (currentRoom >= 0)
	{
		nextStep = rooms[currentRoom].aStar(maze, security_map, p1, target);
	}
	p1.mouve(nextStep);
	if (nextStep->operator==(target));
	{
		action(currentRoom, p1, target);

	}
	A.push_back(p1);

}
Target chooseTarget(Player p1, std::list<Player> B) {
	if (p1.getHealth < 25)
		return findMinTargetInVector(p1, healthStorages);
	if (p1.getAmmo == 0);
	return findMinTargetInVector(p1, ammoStorages);
	return  findMinTargetInVector(p1, createTargetVectorFromPlayers(B));
}
Target findMinTargetInVector(Player p1, std::vector<Target> t)
{
	//find target with minimum distance from t
}

std::vector<Target>  createTargetVectorFromPlayers(std::list<Player> players)
{
	//create vector<Target> fromm vector<Player>
	vector<Target> targets;
	for (auto it = players.begin(); it != players.end(); it++) {
		targets.push_back(*it);
	}
	return targets;
}

void game() {
	int mouves = 0;
	while (!groupA.empty || !groupB.empty)
	{
		if (mouves % 2 == 0)
			play(groupA, groupB);
		else
			play(groupB, groupA);
	}
}
void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("First Example");

	glutDisplayFunc(display); // refresh window function
	glutIdleFunc(idle); // kind of timer function

	glutMouseFunc(mouse);

	glutCreateMenu(menu);
	glutAddMenuEntry("Play", 1);
	//glutAddMenuEntry("Shoot Bullet", 1);
	//glutAddMenuEntry("Throw Grenade", 2);
	//glutAddMenuEntry("Update Security Map", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}
int getColor(Point2D& point) {
	return maze[point.getY()][point.getX()];
}
int getRoom(Point2D* p) {
	return roomMat[p->getX][p->getY];
}
Point2D* findRoomExit(std::vector<Point2D_hg> solution, int currentRoom)
{

	while (!solution.empty)
	{
		Point2D_hg temp = solution.back();
		solution.pop_back();

		if (getRoom(&temp.getPoint()) == currentRoom)
			return &temp.getPoint();
	}
	//TODO
}
Point2D* Astar(Point2D* pos, Point2D targetPoint, int maxG) {
	Point2D lastPos = Point2D(*pos);
	Point2D* last = NULL;
	int currentRoom = getRoom(pos);
	std::vector<Point2D_hg> solution;
	if (maxG == -1 && getColor(targetPoint) == WALL)
		return false;
	if (last != NULL)
	{
		if (targetPoint == *last && *pos == lastPos)
			return findRoomExit(solution, currentRoom);
		delete last;
		solution.clear();
	}
	last = new Point2D(targetPoint);
	Point2D_hg bestPoint;

	priority_queue <Point2D_hg, vector <Point2D_hg>, ComparePoints> pq;
	vector <Point2D_hg> black;
	vector <Point2D_hg> gray;
	bestPoint = Point2D_hg(*pos, targetPoint);
	pq.emplace(bestPoint);
	gray.push_back(bestPoint);
	Point2D_hg* bestPointAsParent, neighborPos_hg;
	Point2D bestPointPos, neighborPos;
	vector<Point2D_hg>::iterator black_iterator;
	vector<Point2D_hg>::iterator gray_iterator;
	do {
		if (pq.empty())
			return false;
		bestPoint = pq.top();
		pq.pop();
		gray_iterator = find(gray.begin(), gray.end(), bestPoint);
		gray.erase(gray_iterator);
		black.push_back(bestPoint);
		bestPointAsParent = new Point2D_hg(bestPoint);
		bestPointPos = bestPointAsParent->getPoint();
		if (getRoom(&bestPointPos) == getRoom(&targetPoint) || maxG == bestPoint.getG()) {
			while (bestPointAsParent->getParent() != NULL)
			{
				solution.push_back(*bestPointAsParent);
				bestPointAsParent = bestPointAsParent->getParent();
			}
			return findRoomExit(solution, currentRoom);
		}

		neighborPos = Point2D(bestPointPos.getX() + 1, bestPointPos.getY());
		if (getColor(neighborPos) == SPACE) {
			int g = bestPointAsParent->getG + security_map[neighborPos.getX][neighborPos.getY] * ALPHA;
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint, g);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.getX() - 1, bestPointPos.getY());
		if (getColor(neighborPos) == SPACE) {
			int g = bestPointAsParent->getG + security_map[neighborPos.getX][neighborPos.getY] * ALPHA;
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint, g);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.getX(), bestPointPos.getY() + 1);
		if (getColor(neighborPos) == SPACE) {
			int g = bestPointAsParent->getG + security_map[neighborPos.getX][neighborPos.getY] * ALPHA;
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint, g);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}

		neighborPos = Point2D(bestPointPos.getX(), bestPointPos.getY() - 1);
		if (getColor(neighborPos) == SPACE) {
			int g = bestPointAsParent->getG + security_map[neighborPos.getX][neighborPos.getY] * ALPHA;
			neighborPos_hg = Point2D_hg(bestPointAsParent, neighborPos, targetPoint, g);
			black_iterator = find(black.begin(), black.end(), neighborPos_hg);
			gray_iterator = find(gray.begin(), gray.end(), neighborPos_hg);
			if (black_iterator == black.end() && gray_iterator == gray.end())
			{
				pq.emplace(neighborPos_hg);
				gray.push_back(neighborPos_hg);
			}
		}
	} while (true);
	return false;
}


bool isStorage(Point2D* pos) {
	return  isHealthStorage || isAmmoStorage;
}

bool isHealthStorage(Point2D* pos) {
	return maze[pos->getY()][pos->getX()] == HEALTH_STORAGE;
}

bool isAmmoStorage(Point2D* pos) {
	return maze[pos->getY()][pos->getX()] == AMMO_STORAGE;
}

bool isTargetAnopponentPlayer(bool isGroupA, Target t) {
	std::list<Player> opponentGroup = isGroupA ? groupA : groupB;
	Player* opponent = NULL;
	for (auto it = opponentGroup.begin(); it != opponentGroup.end(); it++) {
		Player* temp = opponentGroup[*it];
		if (temp->getX() == t.getX() && temp->getY() == t.getY()) {
			opponent = temp;
		}
	}
	return opponent != NULL ? true : false;
}

void action(int roomIndex, Player p1, Target t)
{
	//todo
	//if target.target==opposit team shoot target
	//if target.target ==storage && p1==target refill
	Point2D* targetPos = new Point2D(t.getX(), t.getY());
	bool isGroupA = (std::find(groupA.begin(), groupA.end(), p1) != groupA.end());
	if (isTargetAnopponentPlayer(isGroupA, t)) {
		p1.shoot(t,maze);
	}

	if (isStorage(targetPos) && p1 == t) {
		if (isHealthStorage) {
			p1.refillHealth();
		}
		else {
			p1.refillAmmo();
		}
	}
}

