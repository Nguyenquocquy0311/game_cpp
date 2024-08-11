#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include "Player.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "PowerUp.h"
#include "Enemy.h"

enum GameState { MENU, PLAYING, GAME_OVER };

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void initSDL();
    void closeSDL();
    void handleEvents();
    void update();
    void draw();
    void drawMenu(bool isGameOverMenu = false);
    void resetGame();
    void handleMenuInput(SDL_Keycode key);
    bool checkCollision();
    bool checkPowerUpCollection();

    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
    GameState gameState;
    int selectedMenuOption;
    int selectedGameOverOption;
    Mix_Chunk* jumpSound;
    bool quit;

    Player player;
    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerUps;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    bool gameOver;
    PowerUpType currentPowerUp;
    int powerUpDuration;

    const int minObstacleHeight = 20;
    const int maxObstacleHeight = 100;
    const int obstacleWidth = 20;
    const int powerUpWidth = 20;
    const int powerUpHeight = 20;
    const int groundHeight = 200;

};

#endif // GAME_H
