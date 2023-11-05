#include "particle.h"
#include <memory>
#include <iostream>

void Particle::create(std::string textureID, Vec2 pos, float scale, Color color, int amount, Vec2 dispersion, float fadeTime, Vec2 speed, float gravity, float friction) {
    for (size_t i = 0; i < amount; ++i) {
        auto particle = std::shared_ptr<Particle>(new Particle());
        auto renderer = std::dynamic_pointer_cast<CoolRenderer>(particle->renderer);
        Vec2 textureSize = renderer->loadTexture("texture", "resources/textures/" + textureID + ".png");
        particle->transform->pos = pos + Vec2{(float) GetRandomValue(-dispersion.x, dispersion.x), (float) GetRandomValue(-dispersion.y, dispersion.y)};
        particle->transform->size = textureSize * scale;
        particle->physics->speed = 0.01f * Vec2{(float) GetRandomValue(-speed.x, speed.x), (float) GetRandomValue(-speed.y, speed.y)};
        particle->physics->gravity = gravity;
        particle->physics->friction = friction;
        particle->id = 999;
        if (fadeTime != 0) {
            particle->fadeTimer = Timer::getInstance(fadeTime, false, [particle]() {particle->destroy();});
            particle->fadeTime = fadeTime;
        }
        renderer->addToState("default", "texture", TextureDataBuilder::init(TextureType::TEXTURE, "texture", textureSize).build());
        renderer->setState("default");
        renderer->setColor(color);

        Object::addParticle(particle);
    }
}

std::shared_ptr<Particle> Particle::createFromObject(std::shared_ptr<Object> object)
{
    object->id = 0;
    auto particle = std::shared_ptr<Particle>(new Particle(*object));
    particle->physics = std::make_shared<Physics>();
    particle->collider = nullptr;
    particle->id = 999;
    Object::addParticle(particle);
    return particle;
}

void Particle::update() {
    if (!fadeTimer || !alphaFade) return;
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(this->renderer);
    renderer->setAlpha((fadeTimer->getTimeLeft() / fadeTime) * 255);
}

void Particle::render() {

}

Particle::Particle() {
    renderer = std::make_shared<CoolRenderer>(transform);
    physics = std::make_shared<Physics>();
}

Particle::Particle(const Object& other) : Object(other) {}

ParticleBuilder ParticleBuilder::init(std::string textureID, Vec2 pos, float scale, Color color) {
    ParticleBuilder builder; 
    builder.textureID = textureID;
    builder.pos = pos;
    builder.scale = scale;
    builder.color = color;
    return builder;
}

ParticleBuilder &ParticleBuilder::setAmount(int amount, Vec2 dispersion) {
    this->amount = amount;
    this->dispersion = dispersion;
    return *this;
}

ParticleBuilder &ParticleBuilder::setFadeTime(float fadeTime, float delta, bool alphaFade) {
    this->fadeTime = fadeTime;
    this->fadeTimeDelta = delta;
    this->alphaFade = alphaFade;
    return *this;
}

ParticleBuilder &ParticleBuilder::setPhys(Vec2 speed, float gravity, float friction) {
    this->speed = speed;
    this->gravity = gravity;
    this->friction = friction;
    return *this;
}

void ParticleBuilder::build() {
    for (size_t i = 0; i < amount; ++i) {
        auto particle = std::shared_ptr<Particle>(new Particle());
        auto renderer = std::dynamic_pointer_cast<CoolRenderer>(particle->renderer);
        Vec2 textureSize = renderer->loadTexture("texture", "resources/textures/" + textureID + ".png");
        particle->transform->pos = pos + Vec2{(float) GetRandomValue(-dispersion.x, dispersion.x), (float) GetRandomValue(-dispersion.y, dispersion.y)};
        particle->transform->size = textureSize * scale;
        particle->physics->speed = 0.01f * Vec2{(float) GetRandomValue(-speed.x, speed.x), (float) GetRandomValue(-speed.y, speed.y)};
        particle->physics->gravity = gravity;
        particle->physics->friction = friction;
        particle->id = 999;
        particle->alphaFade = alphaFade;
        if (fadeTime != 0) {
            particle->fadeTimer = Timer::getInstance(fadeTime + 0.001f * GetRandomValue(-fadeTimeDelta, fadeTimeDelta), false, [particle]() {particle->destroy();});
            particle->fadeTime = fadeTime;
        }
        renderer->addToState("default", "texture", TextureDataBuilder::init(TextureType::TEXTURE, "texture", textureSize).build());
        renderer->setState("default");
        renderer->setColor(color);

        Object::addParticle(particle);
    }
}
