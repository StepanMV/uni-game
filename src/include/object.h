#pragma once

#include "renderer.h"
#include "physics.h"
#include "vec2.h"

class Object {
    public:
        Object() = default;
        Object(Vec2 pos, Vec2 size);

        Object(const Object& other);
        Object& operator=(const Object& other);

        Object(Object&& other);
        Object& operator=(Object&& other);

        virtual Object& spawn(Vec2 pos, Vec2 size);
        Object& setMaxSpeeds(double move, double fall, double fly);
        Object& setForces(double gravity, double friction);
        
        bool checkCollision(const Object& other) const;
        Rectangle getCollisionBox(const Object& other) const;


        virtual void update() = 0;
        void render();
        void move();

        Vec2 getPos() const;
        Vec2 getSize() const;

    protected:
        Vec2 pos;
        Vec2 size;

        Renderer renderer;
        Physics physics;
};