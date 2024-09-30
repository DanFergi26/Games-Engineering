#include <SFML/Graphics.hpp>
#include <iostream> // for debugging

using namespace sf;
using namespace std;

Vector2f ballVelocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const float velocityMultiplier = 1.1f;

const Keyboard::Key controls[4] = {
    Keyboard::A,  // Player 1 up
    Keyboard::Z,  // Player 1 down
    Keyboard::Up, // Player 2 up
    Keyboard::Down // Player 2 down
};

const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;

CircleShape ball;
RectangleShape paddles[2];
Font font;
Text scoreText;

// Track the scores
int player1Score = 0;
int player2Score = 0;

void Load() {
    // Set size and origin of paddles
    for (auto& p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of the ball
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);

    // Reset paddle positions
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    // Reset ball position and velocity
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };

    // Load font from file
    if (!font.loadFromFile("res/fonts/RobotoMono-Regular.ttf")) {
        cerr << "Failed to load font!" << endl;
    }

    // Set up score text properties
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(gameWidth / 2.f, 10.f);
}

void Reset() {
    // Reset paddle positions
    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    // Reset ball position and velocity depending on who serves
    ball.setPosition(gameWidth / 2.f, gameHeight / 2.f);
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };

    // Switch server for next reset
    isPlayer1Serving = !isPlayer1Serving;

    // Update score display
    scoreText.setString(to_string(player1Score) + " - " + to_string(player2Score));
    scoreText.setPosition((gameWidth / 2.f) - (scoreText.getLocalBounds().width / 2.f), 10.f);
}

void Update(RenderWindow& window) {
    // Reset clock and calculate delta time
    static Clock clock;
    float dt = clock.restart().asSeconds();

    // Check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit via Esc key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // Handle Player 1 paddle movement
    float direction1 = 0.f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction1--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction1++;
    }
    paddles[0].move(Vector2f(0.f, direction1 * paddleSpeed * dt));

    // Handle Player 2 paddle movement
    float direction2 = 0.f;
    if (Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }
    if (Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }
    paddles[1].move(Vector2f(0.f, direction2 * paddleSpeed * dt));

    // Move the ball
    ball.move(ballVelocity * dt);

    // Check ball collision with top and bottom walls
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight || by < 0) {
        ballVelocity.y *= -velocityMultiplier; // Reverse Y direction
    }

    // Check if ball goes past Player 1 (left side)
    if (bx < 0) {
        player2Score++; // Player 2 scores
        Reset(); // Reset game state
    }
    // Check if ball goes past Player 2 (right side)
    else if (bx > gameWidth) {
        player1Score++; // Player 1 scores
        Reset(); // Reset game state
    }

    // Handle collision with Player 1 paddle (left)
    else if (bx < paddleSize.x + paddleOffsetWall &&
             by > paddles[0].getPosition().y - (paddleSize.y / 2.f) &&
             by < paddles[0].getPosition().y + (paddleSize.y / 2.f)) {
        ballVelocity.x *= -velocityMultiplier; // Reverse X direction
    }
    // Handle collision with Player 2 paddle (right)
    else if (bx > gameWidth - paddleSize.x - paddleOffsetWall &&
             by > paddles[1].getPosition().y - (paddleSize.y / 2.f) &&
             by < paddles[1].getPosition().y + (paddleSize.y / 2.f)) {
        ballVelocity.x *= -velocityMultiplier; // Reverse X direction
    }
}

void Render(RenderWindow& window) {
    // Draw paddles, ball, and score
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(scoreText);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}