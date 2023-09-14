#include "raylib.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

class Player {
    public:
        Player() = default;

        void update();

    private:
        Vector2 plPos = {0, screenHeight - 90};
        Vector2 plSpeed = {0, 0};
        bool direction = true;
        Texture2D texture = LoadTexture("../images/playerRight.png");
};