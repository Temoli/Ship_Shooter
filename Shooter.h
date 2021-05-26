extern Gamebuino gb;

extern const byte SHIP[];
extern const byte CLOUD_BIG[];
extern const byte CLOUD_MEDIUM[];
extern const byte CLOUD_SMALL[];
extern const byte BULLET[];

class alien{
	private:
		int pos_x;
		int pos_y;
		int speed;

		int bullet_speed;

		int life;

	public:
		void create_alien(int type){

		}

		void move_alien(){

		}

		void display_alien(){

		}

		void alien_fire(int & bullet_x, int & bullet_y){

		}

		bool alien_dmg(int bullet_x, int bullet_y){

		}
};

class clouds{
private:
	int pos_x;
	int pos_y;

	byte * cloud;

	int speed = 2;

public: 
	void create_cloud(int type){
		switch (type){
		case 1:
			cloud = CLOUD_BIG;
			break;
		case 2:
			cloud = CLOUD_MEDIUM;
			break;
		case 3:
			cloud = CLOUD_SMALL;
			break;
		}
		pos_x = random(LCDWIDTH, LCDWIDTH + 21);
		pos_y = random(-5, LCDHEIGHT + 10);
	}

	void move_cloud(){
		if(gb.frameCount % 2)
			pos_x -= speed;
	}

	bool cloud_out(){
		if(pos_x < -50)
			return true;
	}

	void display_cloud(){
		if(!(gb.frameCount % 3))
			gb.display.drawBitmap(pos_x, pos_y, cloud);
	}
};

class bullet{
private:
	int pos_x;
	int pos_y;

	int whose_bullet;
	
	int speed = 2;

public:
	bullet(int x, int y, int who_fires){
		pos_x = x;
		pos_y = y;

		whose_bullet = who_fires;
	}

	void move_bullet(){
		pos_x;
	}

	void draw_bullet(){
		gb.display.drawBitmap(pos_x, pos_y, BULLET);
	}

	bool bullet_out(){
		if ((pos_x > LCDWIDTH + 2) || (pos_x < -2))
			return true;
	}

	bool collision(int x, int y, const byte BITMAP[]){
		return gb.collideBitmapBitmap(x, y, BITMAP, pos_x, pos_y, BULLET);
	}
};