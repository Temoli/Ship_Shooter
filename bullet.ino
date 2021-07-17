bullet::bullet(int x, int y, int who_fires){
    switch(who_fires){
        case PLAYER:
            pos_x = x + 2;
            pos_y = y + 3;
            break;
        case ALIEN:
            pos_x = x;
            pos_y = y + 3;
            break;
    }

    whose_bullet = who_fires;
}

bool bullet::bullet_out(){
    if ((pos_x > LCDWIDTH + 2 && whose_bullet == PLAYER) || (pos_x < -4 && whose_bullet == ALIEN))
        return true;
    else return false;
}