// ship.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Ship : public sf::Sprite {
protected:
    sf::IntRect _sprite;
    // Default constructor is hidden
    Ship();
public:
    // Constructor that takes a sprite
    explicit Ship(sf::IntRect ir);
    // Pure virtual destructor -- makes this an abstract class and avoids undefined behaviour
    virtual ~Ship() = 0;
    // Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float &dt);
};

class Invader : public Ship {
public:
    static bool direction;  // Static member to control direction
    static float speed;     // Static member for movement speed

    Invader(sf::IntRect ir, sf::Vector2f pos);
    Invader();
    void Update(const float &dt) override;
};

// Declare the ships vector globally
extern std::vector<Ship *> ships; // Use 'extern' to declare the vector