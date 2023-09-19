#pragma once
#include "object.h"

class Tile: public Object {
    public:
        Tile();

        virtual void render() override;

        ~Tile();
    private:
        //?
};