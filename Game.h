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
#include "Item.h"

using namespace std;

enum GameState { MENU, PLAYING, GAME_OVER };

extern Mix_Music* gBackgroundMusic;
extern Mix_Music* gPlayMusic;
extern Mix_Chunk* gJumpSound;
extern Mix_Chunk* gShootSound;
extern Mix_Chunk* gEnemySpawnSound;
extern Mix_Chunk* gItemCollectSound;
extern Mix_Chunk* gPowerUpCollectSound;
extern Mix_Chunk* gCollisionSound;
extern Mix_Chunk* gCollisionObsSound;
extern Mix_Chunk* gWinSound;

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
    SDL_Surface* gScreenSurface;
    SDL_Surface* gPlayerSurface;
    SDL_Texture* gMenuBackgroundTexture;

    SDL_Texture* gEnemyRunLeftTexture;
    SDL_Texture* gEnemyRunRightTexture;
    SDL_Texture* gEnemyFlyUpTexture;
    SDL_Texture* gEnemyFlyDownTexture;

    SDL_Rect playerRect;
    SDL_Texture* powerUpHighJumpTexture;
    SDL_Texture* powerUpFlyTexture;
    SDL_Texture* powerUpInvincibleTexture;
    SDL_Texture* gAxeTexture;
    SDL_Texture* gMapTexture;
    SDL_Texture* gDrumstickTexture;
    SDL_Texture* gBagTexture;

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
    std::vector<Item> items;

    bool gameOver;
    PowerUpType currentPowerUp;
    int powerUpDuration;
    int bgX;
    int bgSpeed;
    int score;
    int highScore;
    std::string highScoreFile = "highscore.txt";
    int mapCount;

    const int minObstacleHeight = 50;
    const int maxObstacleHeight = 200;
    const int minObstacleWidth = 60;
    const int maxObstacleWidth = 120;
    const int powerUpWidth = 50;
    const int powerUpHeight = 50;
    const int groundHeight = 150;

};

#endif // GAME_H
