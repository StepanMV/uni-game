#pragma once
#include "object.h"

class Particle: public Object {
    public:
        Particle(double _time);
        
        virtual void render() override;

        ~Particle();
    private:
        double time;
};