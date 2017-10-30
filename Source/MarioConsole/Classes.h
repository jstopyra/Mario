//#include <iostream>
//#include <stdio.h>
//#include <time.h>
//#include <conio.h>
//#include <stdlib.h>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <Windows.h>

using namespace std;


const int PLAYER_LIVES = 3;
const int MAX_MAP_X = 242;
const int MAX_MAP_Y = 18;
const int ENEMIES_COUNT = 16;

//render icons------------------
const char PLAYER_BOX = 1;
const char QUESTION_BOX = '?';
const char BRICK_BOX = 176;
const char SOLID_BOX = 219;
const char SHROOM_BOX = 234;
	//render Colors
const int QUESTION_MARK_CLR = 4;
const int ENEMY_CLR = 5;

//------------------------------

const int COIN_POINTS = 200;
const int ENEMY_POINTS = 100;
const int SHROOM_POINTS = 1000;

struct MapClass{
//	char 
	int mapLength = 0;
	int mapHeight = 0;
};

struct ShroomClass{
	int posX = 0;
	int posY = 0;
	int lastPosX = 0;
	int lastPosY = 0;
	double timeSpawned = 0;
	int directionFacing = 1; //1 is right, -1 is left
	bool isAlive = false;
};

struct EnemyClass{
public:
	bool isAlive = true;
	bool isOnScreen = true;
	int posX = 0;
	int posY = 0;
	int lastPosX = 0;
	int lastPosY = 0;
	double lastMove = 0;
};

struct PlayerClass{
public:
	bool isGrounded = false;
	bool wonGame = false;
	bool isAlive = true;
	int posX = 1;
	int posY = 0;
	int lastPosX = 0;
	int lastPosY = 0;
	int livesLeft = PLAYER_LIVES;
	int pointsCollected = 0;
	int coinsCollected = 0;
	bool canJump = false;
	int playerInput = NULL;
	int blocksJumped = 0;
	bool needToJump = false;
	bool wasJumping = false;
	double timeLanded = 0;
	bool upgraded = false;
	int jumpLimit = 4;
	double lastDamaged = 0;

};

struct CoinClass{
	double timeSpawned = 0;
	double lifeTime = 200;
	bool isPickup = false;
	bool isAlive = false;
	int posX = 0;
	int posY = 0;
};

//POINTERS---------------------------
//bool* pGameRunning;

struct GameClass{
	bool gameRunning = true;
	char mapItems[MAX_MAP_Y][MAX_MAP_X];
	double lastTime = 0;
	int lastScreenX = 0;
	int gameSpeed = 100;
	int screenWidth = 40;
	int screenMiddle = screenWidth / 2;
	int rightScreenBorder = 0;
};


int LoadMap(GameClass &game);

void MakePlayer(PlayerClass &player);

void MakeEnemies(char mapItems[][MAX_MAP_X], EnemyClass enemies[]);


bool GameLoop(int gameSpeed, double &lastTime);

void PlayerDied(PlayerClass &player);

void PlayerWon(PlayerClass &player);

void JumpPlayer(char mapItems[][MAX_MAP_X], PlayerClass &player, CoinClass &coin, ShroomClass &shroom);

bool CheckIfGrounded(EnemyClass enemies[], char mapItems[][MAX_MAP_X], PlayerClass &player, CoinClass &coin, ShroomClass &shroom);

void DropPlayer(char mapItems[][MAX_MAP_X], PlayerClass &player);

void CheckInput(GameClass &game, PlayerClass &player, ShroomClass &shroom);

void DamagePlayer(PlayerClass &player);

void CheckRender(GameClass &game, PlayerClass &player, CoinClass &coin, EnemyClass enemies[], ShroomClass &shroom);

void RenderMap(GameClass game, PlayerClass player, EnemyClass enemies[]);

void EnemyMove(EnemyClass enemies[], GameClass &game, PlayerClass &player);

void MoveShroom(ShroomClass &shroom, GameClass &game, PlayerClass &player);

bool ShroomGrounded(ShroomClass &shroom, GameClass &game);

void PickUpShroom(ShroomClass &shroom, PlayerClass &player, GameClass &game);