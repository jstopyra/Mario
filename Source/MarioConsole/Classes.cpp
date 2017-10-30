#include "Classes.h"
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); //Windows console handler, used to change colors.


//creates and mvoes the enemies on the ground
void MakeEnemies(char mapItems[][MAX_MAP_X], EnemyClass enemies[]){

	int enemyCount = 0;
	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{
			if (mapItems[i][j] == 'e'){
				mapItems[i][j] = 'T';
				enemies[enemyCount].posX = j;
				enemies[enemyCount].posY = 13;
				enemies[enemyCount].isAlive = true;
				enemyCount++;
			}
		}
	}
}


//check the frames of the game. to limit game speed
bool GameLoop(int gameSpeed, double &lastTime){
	if (timeGetTime() < lastTime + gameSpeed)
		return false;
	else{
		lastTime = timeGetTime();
		return true;
	}
}

//restart game when player dies
void PlayerDied(PlayerClass &player){
	cout << "You've died!" << endl;
	cout << "Game will restart shortly" << endl;
	float timeDied = timeGetTime();
	while (timeGetTime() < timeDied + 2000){

	}
	player.isAlive = true;
	//return true;
}

//check if the player is grounded.
bool CheckIfGrounded(EnemyClass enemies[], char mapItems[][MAX_MAP_X], PlayerClass &player, CoinClass &coin, ShroomClass &shroom){
	if (mapItems[player.posY + 1][player.posX] == 'T'){
		for (int i = 0; i < ENEMIES_COUNT; i++){
			if (enemies[i].posX == player.posX && enemies[i].posY == player.posY + 1){
				enemies[i].isAlive = false;
				mapItems[player.posY + 1][player.posX] = 't';
				player.pointsCollected += ENEMY_POINTS;
				JumpPlayer(mapItems, player, coin, shroom);
				JumpPlayer(mapItems, player, coin, shroom);
			}
		}
	}
	return (mapItems[player.posY + 1][player.posX] != ' ' && mapItems[player.posY + 1][player.posX] != 't');
}


//checks if there is anything above players head
//if there are blocks, check what block it is and do stuff correspondingly
void JumpPlayer(char mapItems[][MAX_MAP_X], PlayerClass &player, CoinClass &coin, ShroomClass &shroom){
	bool hitCoin = false;
	if (mapItems[player.posY - 1][player.posX] == ' '){
		player.posY -= 1;
		player.blocksJumped++;
	}
	else{
		switch (mapItems[player.posY - 1][player.posX]){
		case 'x':
			//playerPoints++;
			if (player.upgraded)
			mapItems[player.posY - 1][player.posX] = ' ';
			break;
		case '?':
			hitCoin = true;
			coin.posX = player.posX;
			coin.posY = player.posY - 2;
			//CoinPopup()
			player.pointsCollected += COIN_POINTS;
			player.coinsCollected++;
			mapItems[player.posY - 1][player.posX] = 'X';
			break;
		case '1':
			hitCoin = true;
			coin.posX = player.posX;
			coin.posY = player.posY - 2;
			player.pointsCollected += COIN_POINTS;
			player.coinsCollected++;
			mapItems[player.posY - 1][player.posX] = 'X';
			break;
		case '2':
			hitCoin = true;
			coin.posX = player.posX;
			coin.posY = player.posY - 2;
			player.pointsCollected += COIN_POINTS;
			player.coinsCollected++;
			mapItems[player.posY - 1][player.posX] = '1';
			break;
		case '3':
			hitCoin = true;
			coin.posX = player.posX;
			coin.posY = player.posY - 2;
			player.pointsCollected += COIN_POINTS;
			player.coinsCollected++;
			mapItems[player.posY - 1][player.posX] = '2';
			break;
		case 'O':
			hitCoin = true;
			coin.posX = player.posX;
			coin.posY = player.posY - 2;
			player.pointsCollected += COIN_POINTS;
			mapItems[player.posY - 1][player.posX] = 'X';
			player.coinsCollected++;
			break;
		case 'm':
			//playerPoints += COIN_POINTS;
			//playerCoins++;
			mapItems[player.posY - 1][player.posX] = 'X';
			mapItems[player.posY - 2][player.posX] = SHROOM_BOX;
			shroom.isAlive = true;
			shroom.posX = player.posX;
			shroom.posY = player.posY - 2;
			
			break;
		case'}':

			break;
		}
		if (hitCoin){
			hitCoin = false;
			mapItems[coin.posY][coin.posX] = 'o';
			coin.isAlive = true;
			coin.timeSpawned = timeGetTime();
		}
		player.needToJump = false;
		player.blocksJumped = 0;
	}
}


//resets the game if the player won
void PlayerWon(PlayerClass &player){
	cout << "You've WON!" << endl;
	cout << "Game will restart shortly" << endl;
	float timeWon = timeGetTime();
	while (timeGetTime() < timeWon + 2000){

	}
	player.wonGame = false;
	//break;
}

//loads the map from the file and turns it into the game.mapitems array
int LoadMap(GameClass &game){
	std::ifstream fin("MarioLevel.txt");		//opening txt file named acordingly
	if (!fin) return EXIT_FAILURE;		//exiting if the file doesnt exist
	std::vector<std::string> maze;		//creating vector named maze
	std::string line;					//creating string to read the file to
	while (std::getline(fin, line)) {	//reading the files content into the string vector
		maze.push_back(line);
	}


	for (int i = 0; i < MAX_MAP_Y; i++){	//Looping though one array and transfering variables into another
		for (int j = 0; j < MAX_MAP_X; j++){
			game.mapItems[i][j] = maze[i][j];
		}
	}
}

//creates the player object and moves him to the ground level
void MakePlayer(PlayerClass &player){
	player.posY = 13;
//	player.posX = 200;
}

//moves the player down

void DropPlayer(char mapItems[][MAX_MAP_X], PlayerClass &player){
	player.posY++;
}

//checks if the player pressed any key and mvoes the player in that direction
void CheckInput(GameClass &game, PlayerClass &player, ShroomClass &shroom){
	//mapItems[player.posY][player.posX] = ' ';
	if (GetAsyncKeyState(VK_LEFT)){

		if (game.mapItems[player.posY][player.posX -1] == SHROOM_BOX){
			PickUpShroom(shroom, player, game);

			//player.posX += 1;
		}
		if (game.mapItems[player.posY][player.posX - 1] == 'T'){
			DamagePlayer(player);
		}
		if (game.mapItems[player.posY][player.posX - 1] == ' ' || game.mapItems[player.posY][player.posX - 1] == 'O' || game.mapItems[player.posY][player.posX - 1] == 't' || game.mapItems[player.posY][player.posX - 1] == 'u' || game.mapItems[player.posY][player.posX - 1] == 'N' || game.mapItems[player.posY][player.posX - 1] == '^')
			if (player.posX > game.lastScreenX + 1)
				player.posX -= 1;
	}
	if (GetAsyncKeyState(VK_RIGHT)){
		if (game.mapItems[player.posY][player.posX + 1] == SHROOM_BOX){
			PickUpShroom(shroom, player, game);

			//player.posX += 1;
		}
		if (game.mapItems[player.posY][player.posX + 1] == 'T'){
			DamagePlayer(player);
		}
		if (game.mapItems[player.posY][player.posX + 1] == ' ' || game.mapItems[player.posY][player.posX + 1] == 'O' || game.mapItems[player.posY][player.posX + 1] == 't' || game.mapItems[player.posY][player.posX + 1] == 'u' || game.mapItems[player.posY][player.posX + 1] == 'N' || game.mapItems[player.posY][player.posX + 1] == '^'){
			player.posX += 1;
			if (game.mapItems[player.posY][player.posX + 1] == '^')
				player.wonGame = true;
			if (player.posX > game.lastScreenX + game.screenMiddle){
				game.lastScreenX = player.posX - game.screenMiddle;
			}
		}
	}
	if (GetAsyncKeyState(VK_UP)){
		if (player.canJump)
			if (player.isGrounded){
				player.needToJump = true;
				player.posY -= 1;
				player.blocksJumped++;
			}
	}

	if (game.mapItems[player.posY][player.posX] == SHROOM_BOX){
		PickUpShroom(shroom, player, game);
	}
}

//upgrades the player, gives him points and removes the shroom from the map
void PickUpShroom(ShroomClass &shroom, PlayerClass &player, GameClass &game){
	player.upgraded = true;
	player.pointsCollected += 1000;
	shroom.isAlive = false;
	game.mapItems[shroom.posY][shroom.posX] = ' ';
}

//takes away players upgrade or kills the player
void DamagePlayer(PlayerClass &player){
	if (player.lastDamaged + 300 < timeGetTime())
		if (player.upgraded){
			player.upgraded = false;
		}
		else{
			player.isAlive = false;
		}
		player.lastDamaged = timeGetTime();
}

//checks if the render should run
//render will run on players movement, on enemies movement or on shroom movement

void CheckRender(GameClass &game, PlayerClass &player, CoinClass &coin, EnemyClass enemies[], ShroomClass &shroom){
	bool renderMap = false;
	if (player.lastPosX != player.posX || player.lastPosY != player.posY){
		renderMap = true;
		player.lastPosX = player.posX;
		player.lastPosY = player.posY;
	}
	for (int i = 0; i < ENEMIES_COUNT; i++){
		if (enemies[i].posX != enemies[i].lastPosX || enemies[i].posY != enemies[i].lastPosY){
			renderMap = true;
			enemies[i].lastPosX = enemies[i].posX;
			enemies[i].lastPosY = enemies[i].posY;
		}
	}
	if (shroom.lastPosX != shroom.posX || shroom.lastPosY != shroom.posY){
		renderMap = true;
		shroom.lastPosX = shroom.posX;
		shroom.lastPosY = shroom.posY;
	}
	if (renderMap){
		RenderMap(game, player, enemies);
	}
}

//renders the entire map, switches the colors to set colors
void RenderMap(GameClass game, PlayerClass player, EnemyClass enemies[]){
	system("CLS");
	int minPos = 0;
	int maxPos = 0;
	char playerChar = 1;
	if (player.upgraded)
		playerChar = 2;

	for (int i = 0; i < MAX_MAP_Y; i++)
	{
		for (int j = 0; j < MAX_MAP_X; j++)
		{

			game.rightScreenBorder = game.lastScreenX + game.screenWidth;

			if (j == player.posX && i == player.posY){
				SetConsoleTextAttribute(console, 6);
				cout << playerChar;
				SetConsoleTextAttribute(console, 15);
			}
			else if (j > game.lastScreenX && j < game.rightScreenBorder){
				char pipeSide = 92;
				char pipeBody = 186;
				//if (j > 20)
					switch (game.mapItems[i][j]){
					case 'X':
						SetConsoleTextAttribute(console, 3);
						cout << SOLID_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case 'x':
						SetConsoleTextAttribute(console, 6);
						cout << BRICK_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case ' ':
						SetConsoleTextAttribute(console, 1);
						cout << SOLID_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case '?':
						SetConsoleTextAttribute(console, QUESTION_MARK_CLR);
						cout << QUESTION_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case '1':
						SetConsoleTextAttribute(console, QUESTION_MARK_CLR);
						cout << QUESTION_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case '2':
						SetConsoleTextAttribute(console, QUESTION_MARK_CLR);
						cout << QUESTION_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case '3':
						SetConsoleTextAttribute(console, QUESTION_MARK_CLR);
						cout << QUESTION_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case 'I':
						SetConsoleTextAttribute(console, 2);
						cout << pipeBody;
						SetConsoleTextAttribute(console, 15);
						break;
					case 92:
						SetConsoleTextAttribute(console, 2);
						cout << "\\";
						SetConsoleTextAttribute(console, 15);
						break;
					case '/':
						SetConsoleTextAttribute(console, 2);
						cout << "/";
						SetConsoleTextAttribute(console, 15);
						break;
					case '(':
						cout << "\\";
						break;
					case 'O':
						SetConsoleTextAttribute(console, 1);
						cout << SOLID_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case 'm':
						SetConsoleTextAttribute(console, QUESTION_MARK_CLR);
						cout << QUESTION_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					case 'T':
						SetConsoleTextAttribute(console, ENEMY_CLR);
						cout << "T";
						SetConsoleTextAttribute(console, 15);
						break;
					case 'u':
						cout << BRICK_BOX;
						break;
					case '}':
						SetConsoleTextAttribute(console, 1);
						cout << SOLID_BOX;
						SetConsoleTextAttribute(console, 15);
						break;
					default:
						cout << game.mapItems[i][j];
						break;
				}
				//else
				//	cout << game.mapItems[i][j];
					
			}
			
		}
		cout << endl;
	}

	cout << "Players points: " << setfill('0') << setw(4) << player.pointsCollected << endl;
	cout << "Players coins: " << player.coinsCollected << endl;


}

//checks the timing on the movement and moves if there is no wall next to him.
//if there is a player on its way it will damage him
//if the eny is dead it will just render its dead version
void EnemyMove(EnemyClass enemies[], GameClass &game, PlayerClass &player){
	//srand
	if (timeGetTime() < enemies[0].lastMove + 2000){
		return;
	}
	else{
		enemies[0].lastMove = timeGetTime();
		for (int i = 0; i < ENEMIES_COUNT; i++){

			if (enemies[i].isAlive){
				game.mapItems[enemies[i].posY][enemies[i].posX] = ' ';
				int number = rand() % 2;
				if (number == 0)
					if (game.mapItems[enemies[i].posY][enemies[i].posX + 1] == ' ')
						enemies[i].posX++;


				if (number == 1)
					if (game.mapItems[enemies[i].posY][enemies[i].posX - 1] == ' ')
						enemies[i].posX--;
				enemies[0].lastMove = timeGetTime();
				game.mapItems[enemies[i].posY][enemies[i].posX] = 'T';
				if (enemies[i].posX == player.posX && enemies[i].posY == player.posY){
					DamagePlayer(player);
				}
			}
			else{
				game.mapItems[enemies[i].posY][enemies[i].posX] = 't';
			}
		}
	}
}

//checks if there is an empty space to the side and moves there if it is. If there is a wall it will switch movement direction
//if there is a player it will be pickedup by the player
void MoveShroom(ShroomClass &shroom, GameClass &game, PlayerClass &player){
	game.mapItems[shroom.posY][shroom.posX] = ' ';
	if (game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] == ' ' || game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] == 'T' || game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] == 't'){
		shroom.posX += shroom.directionFacing;
	}
	else if (game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] != ' ' && game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] != 't' && game.mapItems[shroom.posY][shroom.posX + shroom.directionFacing] != 'T')
	{
		if (shroom.directionFacing == 1)
			shroom.directionFacing = -1;
		else if (shroom.directionFacing == -1)
			shroom.directionFacing = 1;
	}
	if (!ShroomGrounded(shroom, game)){
		shroom.posY++;
	}
	game.mapItems[shroom.posY][shroom.posX] = SHROOM_BOX;
	if (shroom.posX < game.lastScreenX){
		shroom.isAlive = false;
	}
	if (shroom.posX == player.posX && shroom.posY == player.posY){
		PickUpShroom(shroom, player, game);

	}
}


//checks if there is anything below the shroom. returns that value
bool ShroomGrounded(ShroomClass &shroom, GameClass &game){
	return (game.mapItems[shroom.posY + 1][shroom.posX] != ' ');
}