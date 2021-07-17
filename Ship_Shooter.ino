#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
#define TEST 0 //1 - disable player death and shows free memory
#define CLOUDS 0 //0 - disable clouds
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

int ship_lives = 3;
int score = 0;
int record = 0;
int time_after_death = 20 * 4; //time to wait after death 20 * seconds to wait;

//player bullets
const int PLAYER_BULLETS_COUNT = 10;
bullet *player_bullets[PLAYER_BULLETS_COUNT];
const int BULLET_DELAY_DURATION = (1 * 20) / 4; //ship will be able to shot every x seconds; seconds * 20; seconds * (1000ms / 50ms); 50 because main loop works every 50ms
int bullet_delay_count = BULLET_DELAY_DURATION; //it will be better if player will be able to shoot right after the game starts; later in the code game checks if bulet_dellay_count is equals to BULLET_DELAY_DURATION and allows player to fire

//aliens
const int ALIENS_COUNT = 10;
aliens *aliens_tab[ALIENS_COUNT];

//aliens bullets
const int ALIEN_BULLETS_COUNT = 30;
bullet *alien_bullets[ALIEN_BULLETS_COUNT];
const int ALIEN_FIRE_SPEED = 30;

#if(CLOUDS)
//clouds
const int CLOUDS_COUNT = 5;
clouds *clouds_tab[CLOUDS_COUNT];
#endif

//__________MAIN__________
void setup(){
	gb.begin();
	gb.titleScreen(F("Ship shooter"));
	gb.pickRandomSeed();
	gb.display.setFont(font3x5);

	for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
		player_bullets[i] = nullptr;
	}

	#if(CLOUDS)
	for (int i = 0; i < CLOUDS_COUNT; i++){
		clouds_tab[i] = nullptr;
	}
	#endif

	for (int i = 0; i < ALIENS_COUNT; i++){
		aliens_tab[i] = nullptr;
	}

	for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
		alien_bullets[i] = nullptr;
	}
}

void loop(){
while (gb.update()){ //returns true every 50ms; 20fps
//INPUT
	//go to menu
	if(gb.buttons.repeat(BTN_C, 0))
		gb.titleScreen(F("Space shooter"));

	if(ship_lives){
		//move ship
		if (gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - SHIP_W && gb.buttons.repeat(BTN_DOWN, 0) && ship_y < LCDHEIGHT - SHIP_H){
			ship_x += ship_v;
			ship_y += ship_v;
		} else if (gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - SHIP_W && gb.buttons.repeat(BTN_UP, 0) && ship_y > 0){
			ship_x += ship_v;
			ship_y -= ship_v;
		} else if (gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0 && gb.buttons.repeat(BTN_DOWN, 0) && ship_y < LCDHEIGHT - SHIP_H){
			ship_x -= ship_v;
			ship_y += ship_v;
		} else if (gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0 && gb.buttons.repeat(BTN_UP, 0) && ship_y > 0){
			ship_x -= ship_v;
			ship_y -= ship_v;
		} else if (gb.buttons.repeat(BTN_UP, 0) && ship_y > 0){
			ship_y -= ship_v;
		} else if (gb.buttons.repeat(BTN_DOWN, 0) && ship_y < LCDHEIGHT - SHIP_H){
			ship_y += ship_v;
		} else if (gb.buttons.repeat(BTN_LEFT, 0) && ship_x > 0){
			ship_x -= ship_v;
		} else if (gb.buttons.repeat(BTN_RIGHT, 0) && ship_x < LCDWIDTH - SHIP_W){
			ship_x += ship_v;
		}			

		//fire; create player bullets
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

//LOGIC
		//__________PLAYER__________
		//move bullets and/or delete bullets
		for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
			if (player_bullets[i] != nullptr)
				player_bullets[i] -> move_bullet();
			if (player_bullets[i] != nullptr && player_bullets[i] -> bullet_out()){
				delete player_bullets[i];
				player_bullets[i] = nullptr;
			}
		}
		
		//player alien collision
		#if(!TEST)
		for (int i = 0; i < ALIENS_COUNT; i++){
			if (aliens_tab[i] != nullptr && aliens_tab[i] -> alien_collision(ship_x, ship_y)){
				ship_lives--;
				ship_x = 0;
				ship_y = 21;

				for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
					delete player_bullets[i];
					player_bullets[i] = nullptr;
				}

				for (int i = 0; i < ALIENS_COUNT; i++){
					delete aliens_tab[i];
					aliens_tab[i] = nullptr;
				}

				for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
					delete alien_bullets[i];
					alien_bullets[i] = nullptr;
				}
				break;
			}
		}
		#endif
		
		//player - alien bullet collision
		#if(!TEST)
		for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
			if (alien_bullets[i] != nullptr && gb.collideBitmapBitmap(ship_x, ship_y, SHIP, alien_bullets[i] -> get_x(), alien_bullets[i] -> get_y(), BULLET)){
				ship_lives--;
				ship_x = 0;
				ship_y = 21;

				for (int i = 0; i < PLAYER_BULLETS_COUNT; i++){
					delete player_bullets[i];
					player_bullets[i] = nullptr;
				}

				for (int i = 0; i < ALIENS_COUNT; i++){
					delete aliens_tab[i];
					aliens_tab[i] = nullptr;
				}

				for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
					delete alien_bullets[i];
					alien_bullets[i] = nullptr;
				}
				break;
			}
		}
		#endif
		
		// __________ALIENS__________
		//create alien
		if(!(gb.frameCount % 10)){ //This prevents aliens from forming over other aliens and sending one huge wave of them
			for (int i = 0; i < ALIENS_COUNT; i++){
				if (aliens_tab[i] == nullptr){
					aliens_tab[i] = new aliens(random(1, 11));
					break;
				}
			}
		}
		
		//move alien
		for (int i = 0; i < ALIENS_COUNT; i++){
			if (aliens_tab[i] != nullptr)
				aliens_tab[i] -> move_alien();
			if (aliens_tab[i] != nullptr && aliens_tab[i] -> alien_out()){
				delete aliens_tab[i];
				aliens_tab[i] = nullptr;
			}
		}

		//alien fire
		for (int i = 0; i < ALIENS_COUNT; i++){
			for (int j = 0; j < ALIEN_BULLETS_COUNT; j++){
				if(alien_bullets[j] == nullptr && aliens_tab[i] != nullptr && !(gb.frameCount % ALIEN_FIRE_SPEED)){
					alien_bullets[j] = aliens_tab[i] -> alien_fire();
					break;
				}
			}
		}

		//move alien bullets and/or delete bullets 
		for (int i = 0; i < ALIEN_BULLETS_COUNT; i++){
			if (alien_bullets[i] != nullptr)
				alien_bullets[i] -> move_bullet();
			if (alien_bullets[i] != nullptr && alien_bullets[i] -> bullet_out()){
				delete alien_bullets[i];
				alien_bullets[i] = nullptr;
			}
		}
																								
		//alien - bullet collision
		for (int i = 0; i < ALIENS_COUNT; i++){
			for (int j = 0; j < PLAYER_BULLETS_COUNT; j++){
				if ((aliens_tab[i] != nullptr && aliens_tab[i] -> alien_collision(player_bullets[j] -> get_x(), player_bullets[j] -> get_y(), &player_bullets[j], score))){ //check if &player_bullets[j] can be replaced with player_bullets[j]
					delete aliens_tab[i];
					aliens_tab[i] = nullptr;
				}
			}
		}

		#if(CLOUDS)
		//__________CLOUDS__________
		//create clouds
		if(!(gb.frameCount % 10)){
			for (int i = 0; i < CLOUDS_COUNT; i++){
				if (clouds_tab[i] == nullptr){
					clouds_tab[i] = new clouds(random(1,4));
					break;
				}
			}
		}
		
		//move and/or delete clouds
		for (int i = 0; i < CLOUDS_COUNT; i++){
			if (clouds_tab[i] != nullptr)
				clouds_tab[i] -> move_cloud();
			if (clouds_tab[i] != nullptr && clouds_tab[i] -> cloud_out()){
				delete clouds_tab[i];
				clouds_tab[i] = nullptr;
			}
		}
		#endif

		if (record < score) record = score;
	} //if(ship_lives) END

	//reset game
	if(ship_lives == 0){
		while(time_after_death-- > 8){ //after death pause game for some time
			while(!gb.update());
			gb.display.clear();
			gb.display.cursorX = 27;
			gb.display.cursorY = 18;
			gb.display.println(F("You lost"));
			gb.display.cursorX = 40;
			gb.display.println(time_after_death / 20);
		}
		gb.display.cursorX = 5;
		gb.display.cursorY = 12;
		gb.display.println(F("Do you want to"));
		gb.display.cursorX = 5;
		gb.display.println(F("play again?"));
		gb.display.cursorX = 5;
		gb.display.println(F("  UP - yes"));
		gb.display.cursorX = 5;
		gb.display.println(F("  DOWN - no"));
		if(gb.buttons.repeat(BTN_UP, 0)){
			ship_lives = 3;
			score = 0;
			time_after_death = 20 * 4;
		}
		if(gb.buttons.repeat(BTN_DOWN, 0)){
			ship_lives = 3;
			score = 0;
			time_after_death = 20 * 4;
			gb.titleScreen(F("Ship shooter"));
		}
	} //reset game END

//DRAW
	if(ship_lives){
		gb.display.clear();

		#if(TEST)
		gb.display.println(gb.getFreeRam());
		#endif

		//display lives and score
		if (gb.frameCount % 2){
			//display ship lives
			gb.display.cursorX = 0;
			gb.display.cursorY = 0;
			gb.display.print("Lives: ");
			gb.display.print(ship_lives-1);

			//display score
			gb.display.cursorX = 0;
			gb.display.cursorY = 37;
			gb.display.print("Score: ");
			gb.display.println(score);
			gb.display.print("Record: ");
			gb.display.println(record);
		}

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

		#if(CLOUDS)
		//draw clouds
		for (int i = 0; i < CLOUDS_COUNT; i++){
			if (clouds_tab[i] != nullptr)
				clouds_tab[i] -> draw_cloud();
		}
		#endif
	} //if(ship_lives) END in DRAW
}//gb.update END
}//loop() EN
