#pragma once
#include "object.h"
#include "timer.h"

class ParticleBuilder {
public:
    static ParticleBuilder init(std::string textureID, Vec2 pos, float scale = 1, Color color = WHITE); 
    ParticleBuilder& setAmount(int amount, Vec2 dispersion = {0, 0});
    ParticleBuilder& setFadeTime(float fadeTime, float delta = 0, bool alphaFade = true);
    ParticleBuilder& setPhys(Vec2 speed, float gravity = 0, float friction = 0);

    void build();
    
private:
    std::string textureID;
    Vec2 pos;
    float scale;
    Color color;
    int amount = 1;
    Vec2 dispersion = {0, 0};
    float fadeTime = 0;
    float fadeTimeDelta = 0;
    bool alphaFade = true;
    Vec2 speed = {0, 0};
    float gravity = 0;
    float friction = 0;
};

class Particle: public Object {
public: 
    static void create(std::string textureID, Vec2 pos, float scale = 1, Color color = WHITE, int amount = 1, Vec2 dispersion = {0, 0}, float fadeTime = 0, Vec2 speed = {0, 0}, float gravity = 0, float friction = 0);
    static std::shared_ptr<Particle> createFromObject(std::shared_ptr<Object> object);

    virtual void update() override;
    virtual void render() override;

    virtual void onCollision(std::shared_ptr<Tile> other) override {};
    virtual void onCollision(std::shared_ptr<Enemy> other) override {};
    virtual void onCollision(std::shared_ptr<Projectile> other) override {};
    virtual void onCollision(std::shared_ptr<Player> other) override {};
    virtual bool isCollideable() const override { return false; };
private:
    Particle();
    Particle(const Object& other);

    bool alphaFade = true;
    float fadeTime = 0;
    std::shared_ptr<Timer> fadeTimer;

    friend class UI;
    friend class ScrollMenu;
    friend class ParticleBuilder;
};