#pragma once
#include <vector>

class Animation {
    public:
        Animation(std::string filepath, double _fps);

        void startAnimation();

        ~Animation();
    private:
        std::vector<Texture2D> textures;//std::array
        const double fps;

};