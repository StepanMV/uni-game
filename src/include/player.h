#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

class Player {
    public:
        Player() = default;

        ~Player();

        void update();

    private:
        Vector2 plPos = {0, screenHeight - 90};
        Vector2 plSpeed = {0, 0};
        bool direction = true;
        Texture2D textureRight = LoadTexture("resources/images/playerRight.png");
        Texture2D textureLeft = LoadTexture("resources/images/playerLeft.png");
        Texture2D texture = textureRight;
};