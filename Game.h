#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "Player.h"
#include "Bullet.h"
#include "Obstacle.h"
#include "PowerUp.h"
#include "Enemy.h"

using namespace std;

enum GameState { MENU, PLAYING, GAME_OVER };

extern Mix_Music* gBackgroundMusic;
extern Mix_Chunk* gJumpSound;
extern Mix_Chunk* gShootSound;

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
    void loadHighScore();
    void saveHighScore();
    void updateScore();
    void drawScore();

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* gBulletTexture;
    SDL_Texture* gPlayerTexture = nullptr;
    SDL_Texture* gObstacleTexture = nullptr;
    SDL_Texture* gEnemyRunTexture = nullptr;
    SDL_Texture* gEnemyFlyTexture = nullptr;
    SDL_Texture* gBackgroundTexture = nullptr;
    //SDL_Texture* powerUpHighJumpTexture;
    //SDL_Texture* powerUpFlyTexture;
    //SDL_Texture* powerUpInvincibleTexture;
    SDL_Surface* gScreenSurface;
    SDL_Surface* gPlayerSurface;
    SDL_Texture* gMenuBackgroundTexture;
    SDL_Texture* gPlayButtonTexture;
    SDL_Texture* gExitButtonTexture;

    SDL_Texture* gEnemyRunLeftTexture;
    SDL_Texture* gEnemyRunRightTexture;
    SDL_Texture* gEnemyFlyUpTexture;
    SDL_Texture* gEnemyFlyDownTexture;

    SDL_Rect playerRect;
    SDL_Texture* powerUpHighJumpTexture;
    SDL_Texture* powerUpFlyTexture;
    SDL_Texture* powerUpInvincibleTexture;

    SDL_Texture* loadTexture(string path);

    int width;
    int height;
    GameState gameState;
    int selectedMenuOption;
    int selectedGameOverOption;
    //Mix_Chunk* jumpSound;
    bool quit;

    Player player;
    std::vector<Obstacle> obstacles;
    std::vector<PowerUp> powerUps;
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    bool gameOver;
    PowerUpType currentPowerUp;
    int powerUpDuration;
    int bgX;
    int bgSpeed;
    int score;
    int highScore;
    std::string highScoreFile = "highscore.txt";

    const int minObstacleHeight = 20;
    const int maxObstacleHeight = 200;
    const int minObstacleWidth = 50;
    const int maxObstacleWidth = 100;
    const int powerUpWidth = 50;
    const int powerUpHeight = 50;
    const int groundHeight = 150;

};

#endif // GAME_H
