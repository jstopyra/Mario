#include "Classes.h"

#include <stdio.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
//#include <Windows.h>



int main(){
	//start by creating a Game object
	GameClass game;
	CoinClass coin;
	coin.isPickup = false;
	srand(time(0));
	while (game.gameRunning){
		//get the game starting by loading in the map and creating enemies and players:
		//loading in the game
		LoadMap(game);
		ShroomClass shroom;
		shroom.isAlive = false;
		//create the player
		PlayerClass player;
		MakePlayer(player);
		//create enemies

		EnemyClass enemies[ENEMIES_COUNT];
		MakeEnemies(game.mapItems, enemies);

		//reset all the variables
		//check what time does the game begun at
		game.lastTime = timeGetTime();
		//reset where is the beginning of the screen
		game.lastScreenX = 0;
		while (true){
			if (GameLoop(game.gameSpeed, game.lastTime)){
				if (player.posY > MAX_MAP_Y || !player.isAlive){
					PlayerDied(player);
					break;
				}
				else if (player.wonGame){
					PlayerWon(player);
					break;
				}
				player.isGrounded = CheckIfGrounded(enemies, game.mapItems, player, coin, shroom);
				if (!player.isGrounded){
					if (player.blocksJumped < player.jumpLimit && player.needToJump){
						player.wasJumping = true;
						player.canJump = false;
						JumpPlayer(game.mapItems, player, coin, shroom);
					}
					else{
						player.needToJump = false;
						player.blocksJumped = 0;
						DropPlayer(game.mapItems, player);
					}
				}
				else if (player.isGrounded && player.needToJump){
					player.needToJump = false;
					player.blocksJumped = 0;
				}

				if (player.wasJumping && player.isGrounded){
					player.timeLanded = timeGetTime() + 100;
					player.wasJumping = false;
				}
				if (player.isGrounded && timeGetTime() > player.timeLanded && !player.canJump){
					player.canJump = true;
				}
				
				CheckInput(game, player, shroom);
				if (timeGetTime() > coin.timeSpawned + coin.lifeTime && coin.isAlive){
					game.mapItems[coin.posY][coin.posX] = ' ';
					coin.isAlive = false;
					coin.posY = 100;
				}
				//EnemyMove(enemies, game, player);
				CheckRender(game, player, coin, enemies, shroom);
				EnemyMove(enemies, game, player);
				if (shroom.isAlive){
					MoveShroom(shroom, game, player);
				}
			}
		}

	}
}

