#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
    public:
        Tile() = default;
        Tile(Vec2 pos, Vec2 size, bool _isUp = false, bool _isDown = false, bool _isRight = false, bool _isLeft = false, bool _canClimbLeft = true, bool _canClimbRight = true);
        virtual Tile& spawn(Vec2 pos, Vec2 size) override;
        Tile& setId(unsigned long _id);
        Tile& setForm(unsigned _form);

        unsigned long getId() const;
        unsigned getForm() const;

        virtual void update() override;
        bool isUp = false, isDown = false, isLeft = false, isRight = false;
        bool canClimbLeft = true, canClimbRight = true; 
    private:
        unsigned long id = 1;
        unsigned form = 1;
};