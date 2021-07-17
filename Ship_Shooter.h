extern Gamebuino gb;
extern const byte font3x5[];

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

class clouds{
private:
	int pos_x = random(LCDWIDTH, LCDWIDTH * 2);
	int pos_y = random(-5, LCDHEIGHT + 10);
	int rotation = random(0, 4);
	int flip = random(0, 4);

	byte * cloud;

	int speed = 2;

public:
	clouds(int type);

	void move_cloud(){
		if(gb.frameCount % 2)
			pos_x -= speed;
	}

	bool cloud_out(){
		return pos_x < -40 ? true: false;
	}

	void draw_cloud();
};

class bullet{
private:
	int pos_x;
	int pos_y;

	int whose_bullet; //1 - PLAYER; 2 - ALIEN
	
	int bullet_speed = 2;

public:
	bullet(int x, int y, int who_fires);

	int get_x(){
		return pos_x;
	}
	int get_y(){
		return pos_y;
	}

	void move_bullet(){
		whose_bullet == PLAYER ? pos_x += bullet_speed : pos_x -= bullet_speed;
	}

	void draw_bullet(){
		gb.display.drawBitmap(pos_x, pos_y, BULLET);
	}

	bool bullet_out(); //checks if bullet is out of screen
};

class aliens{
private:
	int type;
	int pos_x;
	int pos_y;
	int speed;
	int point_value;

	byte * alien;

	int bullet_speed;

	int life;

	//used in type 3
	bool bounce;
	int line; //random; after alien reaches top or bottom line this will be reduced with every move. After 0 alien goes only forward

public:
	aliens(int type);

	void move_alien();

	void display_alien(){
		gb.display.drawBitmap(pos_x, pos_y, alien);
	}
	
	bool alien_out(){
		return pos_x < -10 ? true : false;
	}

	bullet *alien_fire(){
		return new bullet(pos_x, pos_y, ALIEN);
	}

	bool alien_collision(int bullet_x, int bullet_y, bullet ** ptr_bullet_obj, int &score);

	bool alien_collision(int bullet_x, int bullet_y){
		return gb.collideBitmapBitmap(bullet_x, bullet_y, BULLET, pos_x, pos_y, alien);
	}
};