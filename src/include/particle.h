#pragma once
#include "object.h"

class Particle: public Object {
    public: 
        virtual void update() override;

        ~Particle();
    private:
        Particle(double _time);
        double time;
};