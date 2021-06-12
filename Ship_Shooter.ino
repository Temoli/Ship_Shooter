#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#define TEST 1 //1 - disable player death
//84x48
const int PLAYER = 1;
const int ALIEN = 2;

#include "Ship_Shooter.h"

//ship
const int SHIP_W = 3;
const int SHIP_H = 5;

int ship_x = 0;
int ship_y = 21;

int ship_v = 1;

int lifes = 3;
//player bullets
const int PLAYER_BULLETS_COUNT = 10;
bullet *player_bullets[PLAYER_BULLETS_COUNT];
const int BULLET_DELAY_DURATION = (1 * 20) / 4; //ship will be able to shot every x seconds; seconds * 20; seconds * (1000ms / 50ms); 50 because main loop works every 50ms
int bullet_delay_count = BULLET_DELAY_DURATION; //it will be better if player will be able to shoot right after the game starts; later in the code game checks if bulet_dellay_count is equals to BULLET_DELAY_DURATION and allows player to fire

//aliens
const int ALIENS_COUNT = 4;
aliens *aliens_tab[ALIENS_COUNT];
//int aliens_alive[ALIENS_COUNT]; //if 1 alien is dead if its <=0 alien is alive at this position x; it prevents aliens from overlapping

//aliens bullets
const int ALIEN_BULLETS_COUNT = 30;
bullet *alien_bullets[ALIEN_BULLETS_COUNT];

//clouds
const int CLOUDS_COUNT = 6;
clouds *clouds_tab[CLOUDS_COUNT];

//######_main_######

void setup(){
	gb.begin();
	gb.titleScreen(F("Ship shooter"));
	gb.pickRandomSeed();

	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		player_bullets[i] = nullptr;
	}

	for (int i = 0; i < CLOUDS_COUNT; i++){
		clouds_tab[i] = nullptr;
	}

	for (int i = 0; i < ALIENS_COUNT; i++){
		aliens_tab[i] == nullptr;
	}

	// for (int i = 0; i < ALIENS_COUNT; i++){
	// 	aliens_alive[i] == 1;
	// }

	for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
		alien_bullets[i] == nullptr;
	}

}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
//INPUT
	if (gb.buttons.repeat(BTN_UP, 0) && ship_y > 0){
		ship_y -= ship_v;
	} else if (gb.buttons.repeat(BTN_DOWN, 0) && ship_y < LCDHEIGHT - SHIP_H){
		ship_y += ship_v;
	} else if (gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0){
		ship_x -= ship_v;
	} else if (gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - SHIP_W){
		ship_x += ship_v;
	}

	//create player bullets
	if(bullet_delay_count < BULLET_DELAY_DURATION) bullet_delay_count++;
	if (gb.buttons.repeat(BTN_A, 0) && bullet_delay_count == BULLET_DELAY_DURATION){
		for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
			if (player_bullets[i] == nullptr){
				player_bullets[i] = new bullet(ship_x, ship_y, PLAYER);
				bullet_delay_count = 0;
				break;
			}
		}	
	}

	//go to menu
	if(gb.buttons.repeat(BTN_C, 0) || lifes <= 0)
		gb.titleScreen(F("Space shooter"));

//LOGIC
	//__________PLAYER__________
	//move bullets and/or delete bullets
	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		if (player_bullets[i] != nullptr)
			player_bullets[i] -> move_bullet();
		if (player_bullets[i] -> bullet_out()){
			delete player_bullets[i];
			player_bullets[i] = nullptr;
		}
	}
	//player alien collision
	#if(!TEST)
	for (int i = 0; i < ALIENS_COUNT; i++){
		if (aliens_tab[i] -> alien_collision(ship_x, ship_y)){
			lifes--;
			ship_x = 0;
			ship_y = 21;
			//#TO_DO add pop up message with ramaining lifes; maybe copy int lifes to some string
			//gb.popup("Lifes: " + lifes, 20 * 2);
		}
	}
	#endif
	//player - alien bullet collision
	//#TO_DO
	#if(!TEST)
	for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
		if (gb.collideBitmapBitmap(ship_x, ship_y, SHIP, alien_bullets[i] -> get_x(), alien_bullets[i] -> get_y(), BULLET)){
			lifes--;
			ship_x = 0;
			ship_y = 21;
			//gb.popup("Lifes: " + lifes, 20 * 2);
		}
	}
	#endif
	
	// __________ALIENS__________
	//create alien
	for (int i = 0; i < ALIENS_COUNT; i++){
		if (aliens_tab[i] == nullptr){
			aliens_tab[i] = new aliens(random(1, 4));
		}
	}
	
	//move alien
	for (int i = 0; i < ALIENS_COUNT; i++){
		if (aliens_tab[i] != nullptr)
			aliens_tab[i] -> move_alien();
		if (aliens_tab[i] -> alien_out()){
			delete aliens_tab[i];
			aliens_tab[i] = nullptr;
		}
	}

	for (int i = 0; i < ALIENS_COUNT; i++){
		for (int j = 0; j < ALIEN_BULLETS_COUNT; j++){
			if(alien_bullets[j] == nullptr && aliens_tab[i] != nullptr && !(gb.frameCount % 20)){
				alien_bullets[j] = aliens_tab[i] -> alien_fire();
				break;
			}
		}
	}

	//move alien bullets and/or delete bullets 
	for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
		if (alien_bullets[i] != nullptr)
			alien_bullets[i] -> move_bullet();
		if (alien_bullets[i] -> bullet_out()){
			delete alien_bullets[i];
			alien_bullets[i] = nullptr;
		}
	}

																							 
	// //alien - bullet collision
	// for (int i = 0; i < ALIENS_COUNT; i++){
	// 	for (int j = 0; j < PLAYER_BULLETS_COUNT; j++){
	// 		if ((aliens_tab[i] -> alien_collision(player_bullets[j] -> get_x(), player_bullets[j] -> get_y()))){ //probably x+2 to read bullets right end; hit alien and if alien is dead delete it
	// 			delete aliens_tab[i];
	// 			aliens_tab[i] = nullptr;
	// 		}
	// 	}
	// }

	//__________CLOUDS__________
	// //create clouds
	// for (int i = 0; i < CLOUDS_COUNT; i++){
	// 	if (clouds_tab[i] == nullptr){
	// 		clouds_tab[i] = new clouds(random(1,4));
	// 	}
	// }
	// //move and/or delete clouds
	// for (int i = 0; i < CLOUDS_COUNT; i++){
	// 	if (clouds_tab[i] != nullptr)
	// 		clouds_tab[i] -> move_cloud();
	// 	if (clouds_tab[i] -> cloud_out()){
	// 		delete clouds_tab[i];
	// 		clouds_tab[i] = nullptr;
	// 	}
	// }

//DRAW
	gb.display.clear();

	gb.display.drawBitmap(ship_x, ship_y, SHIP);



	//draw player bullets
	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		if (player_bullets[i] != nullptr)
			player_bullets[i] -> draw_bullet();
	}

	//draw alien
	for (int i = 0; i < ALIENS_COUNT; i++){
		if (aliens_tab[i] != nullptr)
			aliens_tab[i] -> display_alien();
	}

	//draw alien bullets
	for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
		if (alien_bullets[i] != nullptr)
			alien_bullets[i] -> draw_bullet();
	}

	//draw clouds
	for (int i = 0; i < CLOUDS_COUNT; i++){
		if (clouds_tab[i] != nullptr)
			clouds_tab[i] -> draw_cloud();
	}


}//gb.update END
}//loop() END
