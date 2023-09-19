#pragma once

class Physics {
    public:
        Physics(int* _x, int* _y);

        void falling();
        void setGravity(bool enable);
        void applyForce();
        void target(int& x, int& y);
        void jump();//
        void flight();//объединить с jump()?
        void apply();

        ~Physics();
    private:
        int *x, *y;// Vec2
        int xSpeed, ySpeed;//Vec2
        int xAcc, yAcc;//Vec2
};