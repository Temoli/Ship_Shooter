#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#include "Shooter.h"
//84x48
const int PLAYER = 1;
const int ALIEN = 2;

//ship
const int SHIP_W = 3;
const int SHIP_H = 5;

int ship_x = 0;
int ship_y = 21;

int ship_v = 1;

//bullets
const int PLAYER_BULLETS_COUNT = 10;
bullet *player_bullets[PLAYER_BULLETS_COUNT];


// clouds cloud_1;
// clouds cloud_2;
// clouds cloud_3;

void setup(){
	gb.begin();
	gb.titleScreen(F("Ship shooter"));
	gb.pickRandomSeed();

	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		player_bullets[i] = nullptr;
	}

	// cloud_1.create_cloud(1);
	// cloud_2.create_cloud(2);
	// cloud_2.create_cloud(3);

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

	if (gb.buttons.repeat(BTN_A, 0)){
		for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
			if (player_bullets[i] == nullptr){
				player_bullets[i] = new bullet(ship_x + 2, ship_y + 3, PLAYER);
				break;
			}
		}	
	}

	//LOGIC

	//DRAW
	gb.display.clear();

	gb.display.drawBitmap(ship_x, ship_y, SHIP);

	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		if (player_bullets[i] != nullptr)
			player_bullets[i] -> draw_bullet();
	}





	// cloud_1.move_cloud();
	// cloud_1.display_cloud();
	// cloud_2.move_cloud();
	// cloud_2.display_cloud();
	// cloud_3.move_cloud();
	// cloud_3.display_cloud();
	
}//gb.update END
}//loop() END
