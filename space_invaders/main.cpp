#include <SFML/Graphics.hpp>
#include <iostream>
#include "ship.h"
#include "game.h"

using namespace std;

sf::Texture spritesheet; 
sf::RenderWindow window(sf::VideoMode(800, 600), "Space Invaders");

void Load() {
    if (!spritesheet.loadFromFile("C:/Users/danfe/source/repos/Games-Engineering/res/sprites/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << endl;
        return;
    }

    const int invaderWidth = 32;
    const int invaderHeight = 32;
    const int spacing = 10; 

    for (int row = 0; row < invaders_rows; ++row) {
        for (int col = 0; col < invaders_columns; ++col) {
            float x = col * (invaderWidth + spacing); 
            float y = row * (invaderHeight + spacing); 

            Invader* inv = new Invader(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(32, 32)), sf::Vector2f(x + 100, y + 100));
            ships.push_back(inv);
        }
    }
}

void Render() {
    window.clear();
    
    for (const auto& s : ships) {
        window.draw(*s);
    }

    window.display();
}

void Update(float dt) {
    for (auto& s : ships) {
        s->Update(dt);
    }
}

int main() {
    Load();

    sf::Clock clock; // Create a clock to measure time

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds(); // Calculate delta time

        Update(dt); // Pass delta time to update
        Render();
    }

    // Cleanup dynamically allocated memory (optional)
    for (auto ship : ships) {
        delete ship;
    }
    ships.clear();

    return 0;
}