#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include "Ship_Shooter.h"
//84x48
const int PLAYER = 1;
const int ALIEN = 2;

//ship
const int SHIP_W = 3;
const int SHIP_H = 5;

int ship_x = 0;
int ship_y = 21;

int ship_v = 1;

//player bullets
const int PLAYER_BULLETS_COUNT = 10;
bullet *player_bullets[PLAYER_BULLETS_COUNT];
const int BULLET_DELAY_DURATION = (1 * 20) / 4; //ship will be able to shot every x seconds; seconds * 20; seconds * (1000ms / 50ms); 50 because main loop works every 50ms
int bullet_delay_count = BULLET_DELAY_DURATION; //it will be better if player will be able to shoot right after the game starts; later in the code game checks if bulet_dellay_count is equals to BULLET_DELAY_DURATION and allows player to fire


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
				player_bullets[i] = new bullet(ship_x + 2, ship_y + 3, PLAYER);
				bullet_delay_count = 0;
				break;
			}
		}	
	}

	//LOGIC
	//move bullets and/or delete bullets
	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		if (player_bullets[i] != nullptr)
			player_bullets[i] -> move_bullet();
		if (player_bullets[i] -> bullet_out()){
			delete player_bullets[i];
			player_bullets[i] = nullptr;
		}
	}

	//create, move and delete clouds
	for (int i = 0; i < CLOUDS_COUNT; i++){
		if (clouds_tab[i] == nullptr){
			clouds_tab[i] = new clouds(random(1,4));
		}
	}
	for (int i = 0; i < CLOUDS_COUNT; i++){
		if (clouds_tab[i] != nullptr)
			clouds_tab[i] -> move_cloud();
		if (clouds_tab[i] -> cloud_out()){
			delete clouds_tab[i];
			clouds_tab[i] = nullptr;
		}
	}

	//DRAW
	gb.display.clear();

	gb.display.drawBitmap(ship_x, ship_y, SHIP);

	//draw bullets
	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		if (player_bullets[i] != nullptr)
			player_bullets[i] -> draw_bullet();
	}

	//draw clouds
	for (int i = 0; i < CLOUDS_COUNT; i++){
		if (clouds_tab[i] != nullptr)
			clouds_tab[i] -> draw_cloud();
	}


}//gb.update END
}//loop() END
