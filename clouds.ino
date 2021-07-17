clouds::clouds(int type){
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

void clouds::draw_cloud(){
    if(!(gb.frameCount % 3))
        gb.display.drawBitmap(pos_x, pos_y, cloud, rotation, flip);
}