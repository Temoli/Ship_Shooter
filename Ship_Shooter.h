extern Gamebuino gb;

extern const int PLAYER;
extern const int ALIEN;
extern const byte SHIP[];
extern const byte CLOUD_BIG[];
extern const byte CLOUD_MEDIUM[];
extern const byte CLOUD_SMALL[];
extern const byte BULLET[];
extern const byte ALIEN_1[];
extern const byte ALIEN_2[];
extern const byte ALIEN_3[];

class aliens{
private:
	int type;
	int pos_x;
	int pos_y;
	int speed;

	byte * alien;

	int bullet_speed;

	int life;

	//used in type 3
	bool bounce;
	int line;

public:
	aliens(int type){
		switch (type){
			case 1:
				alien = ALIEN_1;
				this->type = type;
				life = 1;

				speed = 2;

				pos_x = LCDWIDTH + 5;
				pos_y = random(0, LCDHEIGHT - 7);
				break;
			case 2:
				alien = ALIEN_2;
				this->type = type;
				life = 2;
				
				speed = 2;

				pos_x = LCDWIDTH + 5;
				pos_y = random(0, LCDHEIGHT - 7);
				break;
			case 3:
				alien = ALIEN_3;
				this->type = type;
				life = 3;
				
				speed = 2;

				pos_x = LCDWIDTH + 5;
				pos_y = random(0, LCDHEIGHT - 7);

				line = random(0, LCDHEIGHT - 9);
				bounce = false;
				break;
		}
	}

	void move_alien(){
		switch (type){
			case 1:
				pos_x += speed;
				break;
			case 2:
				pos_x += abs(speed);
				pos_y += speed;
				if (pos_y <= 0){
					speed = -speed;
				} else if (pos_y >= LCDHEIGHT){
					speed = -speed;
				}
			case 3:
				pos_x += abs(speed);
				if (bounce)
					line--;
				if (line && !bounce) //fly forward if bounced from edge
					pos_y += speed;
				
				if (!bounce && pos_y <= 0){ //fly forward and to edge
					speed = -speed;
					bounce = true;
				} else if (!bounce && pos_y >= LCDHEIGHT){
					speed = -speed;
					bounce = true;
				}

		}
	}

	void display_alien(){
		gb.display.drawBitmap(pos_x, pos_y, alien);
	}

	bool alien_out(){
		if(pos_x < -20)
			return true;
		else return false;
	}

	void alien_fire(int & bullet_x, int & bullet_y){

	}

	bool alien_collision(int bullet_x, int bullet_y){

	}
};

class clouds{
private:
	int pos_x = random(LCDWIDTH, LCDWIDTH * 2);
	int pos_y = random(-5, LCDHEIGHT + 10);

	int rotation = random(0, 4);
	int flip = random(0, 4);

	byte * cloud;

	int speed = 2;

public:
	clouds(int type){
		switch (type){
		case 1:
			cloud = CLOUD_BIG;
			break;
		case 2:
			cloud = CLOUD_MEDIUM;
			break;
		case 3:
			cloud = CLOUD_SMALL;
			speed = 3;
			break;
		}
	}

	void move_cloud(){
		if(gb.frameCount % 2)
			pos_x -= speed;
	}

	bool cloud_out(){
		if(pos_x < -40)
			return true;
		else return false;
	}

	void draw_cloud(){
		if(!(gb.frameCount % 3))
			gb.display.drawBitmap(pos_x, pos_y, cloud, rotation, flip);
	}
};

class bullet{
private:
	int pos_x;
	int pos_y;

	int whose_bullet;
	
	int bullet_speed = 2;

public:
	bullet(int x, int y, int who_fires){
		pos_x = x;
		pos_y = y;

		whose_bullet = who_fires;
	}

	int get_x(){
		return pos_x;
	}
	int get_y(){
		return pos_y;
	}

	void move_bullet(){
		pos_x += bullet_speed;
	}

	void draw_bullet(){
		gb.display.drawBitmap(pos_x, pos_y, BULLET);
	}

	bool bullet_out(){
		if ((pos_x > LCDWIDTH + 2 && whose_bullet == PLAYER) || (pos_x < -2 && whose_bullet == ALIEN))
			return true;
		else return false;
	}

	// bool collision(int x, int y, const byte BITMAP[]){
	// 	return gb.collideBitmapBitmap(x, y, BITMAP, pos_x, pos_y, BULLET);
	// }
};
