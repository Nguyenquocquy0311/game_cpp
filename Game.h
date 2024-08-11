#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    void initSDL();
    void closeSDL();
    void loadTextures();
    void drawMenu();
    void draw();
    void update();
    void generateObstacles();
    void generateEnemies();
    void generatePowerUps();
    bool checkCollision();
    bool checkPowerUpCollection();

    enum PowerUpType { NONE, HIGH_JUMP, LONG_JUMP, INVINCIBLE };
    enum GameState { MENU, PLAYING, GAME_OVER };

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* playerTexture;
    SDL_Texture* invincibleTexture;
    SDL_Texture* highJumpTexture;
    SDL_Texture* longJumpTexture;
    SDL_Texture* backgroundTexture;
    SDL_Rect playerRect;
    std::vector<SDL_Rect> obstacleRects;
    std::vector<SDL_Rect> powerUpRects;
    std::vector<SDL_Rect> enemyRects;
    std::vector<PowerUpType> powerUpTypes;
    bool isJumping;
    bool isFalling;
    bool gameOver;
    int jumpSpeed;
    int fallSpeed;
    PowerUpType currentPowerUp;
    int powerUpDuration;
    GameState gameState;
    int selectedMenuOption;
};

#endif