#include "player.h"
#include <iostream>

void Player::update() {
    plPos.x += plSpeed.x;
    plPos.y += plSpeed.y;
    DrawTextureEx(texture, plPos, 0, 0.1, WHITE);
    if((plSpeed.x > 0) && (direction == false)) {
        direction = true;
        texture = LoadTexture("resources/images/playerRight.png");
    }
    if((plSpeed.x < 0) && (direction == true)) {
        direction = false;
        texture = LoadTexture("resources/images/playerLeft.png");
    }
    if((IsKeyDown(KEY_LEFT) && IsKeyDown(KEY_RIGHT)) || (!IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT))) {
        if(plSpeed.x > 0.5) {
            plSpeed.x -= 0.5;
        }
        else if(plSpeed.x < -0.5) {
            plSpeed.x += 0.5;
        }
        else {
            plSpeed.x = 0;
        }

    }
    else if(IsKeyDown(KEY_LEFT)) {
        if(plSpeed.x > -10) {
            plSpeed.x -= 0.2;
        }
        else {
            plSpeed.x = -10;
        }
    }
    else if(IsKeyDown(KEY_RIGHT)) {
        if(plSpeed.x < 10) {
            plSpeed.x += 0.2;
        }
        else {
            plSpeed.x = 10;
        }
    }
    if(plPos.x > screenWidth - 90) {
        plPos.x = screenWidth - 90;
        plSpeed.x = 0;
    }
    if(plPos.x < 0) {
        plPos.x = 0;
        plSpeed.x = 0;
    }
    if(plPos.y < screenHeight - 90) {
        plSpeed.y += 0.2;
    }
    else {
        plSpeed.y = 0;
    }
    if(IsKeyPressed(KEY_SPACE)) {
        plSpeed.y = -8;
    }
    if(plPos.y > screenHeight - 90) {
        plPos.y = screenHeight - 90;
    }
}