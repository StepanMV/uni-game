#pragma once
#include "object.h"
#include "vec2.h"

class Tile: public Object {
    public:
        Tile() = default;
        Tile(Vec2 pos, Vec2 size);
        virtual Tile& spawn(Vec2 pos, Vec2 size) override;
        Tile& setId(unsigned long _id);
        Tile& setForm(unsigned _form);

        unsigned long getId() const;
        unsigned getForm() const;

        virtual void update() override;
    private:
        unsigned long id = 1;
        unsigned form = 1;
};