aliens::aliens(int type){
    if(type < 7)
        type = 1;
    else if (type < 9)
        type = 2;
    else type = 3;

    switch (type){
        case 1:
            alien = ALIEN_1;
            this->type = type;
            life = 2;
            point_value = 1;

            speed = 1;

            pos_x = LCDWIDTH + 2;
            pos_y = random(0, LCDHEIGHT - 7);
            break;
        case 2:
            alien = ALIEN_2;
            this->type = type;
            life = 3;
            point_value = 2;
            
            random(0,1) ? speed = 1: speed = -1;

            pos_x = LCDWIDTH + 2;
            pos_y = random(1, LCDHEIGHT - 7); //if it was random(0, LCDHEIGHT - 7) some aliens were blocked at the top of the screen and flickering
            break;
        case 3:
            alien = ALIEN_3;
            this->type = type;
            life = 4;
            point_value = 3;
            
            random(0,2) ? speed = 1: speed = -1;

            pos_x = LCDWIDTH + 2;
            pos_y = random(0, LCDHEIGHT - 7);

            line = random(0, 31);
            bounce = false;
            break;
    }
}

void aliens::move_alien(){
    switch (type){
        case 1: //flies only forward
            pos_x -= speed;
            break;
        case 2: //flies diagonally and bounces from top and bottom edges
            pos_x -= abs(speed);
            pos_y += speed;
            if (pos_y <= 0){
                speed = -speed;
            } else if (pos_y >= LCDHEIGHT - 8){
                speed = -speed;
            }
            break;
        case 3: //flies diagonally, bounces once from top or bottom edge, speeds up a bit, flies diagonally random numbers of lines and then flies forward
            pos_x -= abs(speed);

            if(!bounce){ //if not bounced fly up/down till reached screen edge
                pos_y += speed;
                if (pos_y <= 0){
                    speed = speed * -2;
                    bounce = true;
                } else if (pos_y >= LCDHEIGHT - 8){
                    speed = speed * -2;
                    bounce = true;
                }
            }

            if(bounce && line-- > 0){ //if bounced fly up/dwn a few more lines
                pos_y += speed;
            }
            break;
    }
}

bool aliens::alien_collision(int bullet_x, int bullet_y, bullet ** ptr_bullet_obj, int &score){
    if (gb.collideBitmapBitmap(bullet_x, bullet_y, BULLET, pos_x, pos_y, alien)){
        life--;
        delete *ptr_bullet_obj;
        *ptr_bullet_obj = nullptr;
    }
    if (life <= 0){
        score += point_value;
        return true;
    } else return false;
}
