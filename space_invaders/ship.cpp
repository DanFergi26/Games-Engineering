// ship.cpp
#include "ship.h"
#include "game.h"

using namespace sf;
using namespace std;

// Define static members of Invader
bool Invader::direction = true;  // Initialize direction (right by default)
float Invader::speed = 50.0f;    // Initialize speed (can be adjusted as needed)

// Define the ships vector
std::vector<Ship *> ships; // Define the ships vector

Ship::Ship() {}

Ship::Ship(IntRect ir) : Sprite() {
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
}

void Ship::Update(const float &dt) {}
Ship::~Ship() = default;

Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) {
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
}

void Invader::Update(const float &dt) {
    Ship::Update(dt);

    // Move the invader horizontally based on the direction and speed
    move(Vector2f(dt * (direction ? 1.0f : -1.0f) * speed, 0.0f));

    // Check if the invader has reached the window boundaries
    if ((direction && getPosition().x > gameWidth - 16) || (!direction && getPosition().x < 16)) {
        direction = !direction;  // Change direction

        // Move all invaders down by 24 units
        for (auto& s : ships) {
            s->move(Vector2f(0.0f, 24.0f));
        }
    }
}

