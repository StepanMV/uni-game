#include "particle.h"
#include <memory>

std::shared_ptr<Particle> Particle::create(unsigned id, Vec2 pos, Vec2 size) {
    auto particle = std::shared_ptr<Particle>(new Particle());
    particle->id = id;
    particle->transform->pos = pos;
    particle->transform->size = size;
    auto renderer = std::dynamic_pointer_cast<CoolRenderer>(particle->renderer);
    Vec2 textureSize = renderer->loadTexture("texture", "resources/textures/Particle_" + std::to_string(id) + ".png");
    renderer->addToState("main", "texture", TextureDataBuilder::init(TextureType::TEXTURE, "texture", textureSize).build());
    renderer->setState("main");
    Object::addParticle(particle);
    return particle;
}

std::shared_ptr<Particle> Particle::createFromObject(std::shared_ptr<Object> object) {
    object->id = 0;
    auto particle = std::shared_ptr<Particle>(new Particle(*object));
    particle->physics = std::make_shared<Physics>();
    particle->collider = nullptr;
    particle->id = 999;
    Object::addParticle(particle);
    return particle;
}

void Particle::update() {
}

void Particle::render() {

}

Particle::Particle() {
    renderer = std::make_shared<CoolRenderer>(transform);
    physics = std::make_shared<Physics>();
}

Particle::Particle(const Object& other) : Object(other) {}