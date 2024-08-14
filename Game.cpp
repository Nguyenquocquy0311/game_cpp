#include "Game.h"
#include "Player.h"
#include "Enemy.h"

#include <SDL_ttf.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

Mix_Music* gBackgroundMusic = nullptr;
Mix_Chunk* gJumpSound = nullptr;
Mix_Chunk* gShootSound = nullptr;


Game::Game() : width(1200), height(700), window(nullptr), renderer(nullptr), gameState(MENU), selectedMenuOption(0),
               selectedGameOverOption(0), quit(false), gameOver(false), currentPowerUp(NONE), powerUpDuration(0), player(width, height),
               bgX(0), bgSpeed(2) {
    initSDL();
}


Game::~Game() {
    closeSDL();
}

SDL_Texture* Game::loadTexture(string path) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void Game::initSDL() {
    // Image
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Conqueror's Journey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    gScreenSurface = SDL_GetWindowSurface(window);
    if (gScreenSurface == nullptr) {
        std::cerr << "Screen surface could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }

    gPlayerTexture = loadTexture("asset/img/player.png");
    if (gPlayerTexture == nullptr) {
        cerr << "Failed to load player image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Player image loaded successfully!" << endl;
    }

    gBulletTexture = loadTexture("asset/img/bullet2.png");
    if (gBulletTexture == nullptr) {
        cerr << "Failed to load bullet image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Bullet image loaded successfully!" << endl;
    }

    gObstacleTexture = loadTexture("asset/img/obstacle.png");
    if (gObstacleTexture == nullptr) {
        cerr << "Failed to load obstacle image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Obstacle image loaded successfully!" << endl;
    }

    gEnemyRunTexture = loadTexture("asset/img/enemy-run-1.png");
    if (gEnemyRunTexture == nullptr) {
        cerr << "Failed to load enemy image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Enemy image loaded successfully!" << endl;
    }

    gEnemyFlyTexture = loadTexture("asset/img/enemy-fly-1.png");
    if (gEnemyFlyTexture == nullptr) {
        cerr << "Failed to load enemy image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Enemy image loaded successfully!" << endl;
    }

    gBackgroundTexture = loadTexture("asset/img/bg-1.jpg");
    if (gBackgroundTexture == nullptr) {
        std::cerr << "Failed to load background image! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    powerUpHighJumpTexture = loadTexture("asset/img/power-up2.png");
    powerUpFlyTexture = loadTexture("asset/img/power-up1.png");
    powerUpInvincibleTexture = loadTexture("asset/img/power-up3.png");

    if (!powerUpHighJumpTexture || !powerUpFlyTexture || !powerUpInvincibleTexture) {
        std::cerr << "Failed to load power-up images!" << std::endl;
        exit(1);
    } else {
        cerr << "Powerup image loaded successfully!" << endl;
    }

    gMenuBackgroundTexture = loadTexture("asset/img/menu.jpg");
    gPlayButtonTexture = loadTexture("asset/img/play-btn.png");
    gExitButtonTexture = loadTexture("asset/img/play-btn.png");

    // Sound
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gBackgroundMusic = Mix_LoadMUS("asset/sound/bg-music.mp3");
    if (gBackgroundMusic == nullptr) {
        std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gJumpSound = Mix_LoadWAV("asset/sound/jump.wav");
    if (gJumpSound == nullptr) {
        std::cerr << "Failed to load jump sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gShootSound = Mix_LoadWAV("asset/sound/shoot.wav");
    if (gShootSound == nullptr) {
        std::cerr << "Failed to load shoot sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }
}


void Game::closeSDL() {
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = nullptr;

    SDL_DestroyTexture(gMenuBackgroundTexture);
    gMenuBackgroundTexture = nullptr;

    SDL_DestroyTexture(gPlayButtonTexture);
    gPlayButtonTexture = nullptr;

    SDL_DestroyTexture(gExitButtonTexture);
    gExitButtonTexture = nullptr;

    SDL_DestroyTexture(gPlayerTexture);
    gPlayerTexture = nullptr;

    SDL_DestroyTexture(gBulletTexture);
    gBulletTexture = nullptr;

    SDL_DestroyTexture(gObstacleTexture);
    gObstacleTexture = nullptr;

    SDL_DestroyTexture(gEnemyRunTexture);
    gEnemyRunTexture = nullptr;

    SDL_DestroyTexture(gEnemyFlyTexture);
    gEnemyFlyTexture = nullptr;

    SDL_DestroyTexture(powerUpHighJumpTexture);
    powerUpHighJumpTexture = nullptr;

    SDL_DestroyTexture(powerUpFlyTexture);
    powerUpFlyTexture = nullptr;

    SDL_DestroyTexture(powerUpInvincibleTexture);
    powerUpInvincibleTexture = nullptr;

    Mix_FreeMusic(gBackgroundMusic);
    gBackgroundMusic = nullptr;

    Mix_FreeChunk(gJumpSound);
    gJumpSound = nullptr;

    Mix_FreeChunk(gShootSound);
    gShootSound = nullptr;

    Mix_CloseAudio();

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    while (!quit) {
        handleEvents();
        if (gameState == MENU) {
            if (Mix_PlayingMusic() == 0) {  // Kiểm tra nếu chưa phát nhạc
                if (Mix_PlayMusic(gBackgroundMusic, -1) == -1) {
                    std::cerr << "Failed to play menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    exit(1);
                }
            }
            drawMenu();
        } else if (gameState == PLAYING) {
            if (Mix_PlayingMusic() != 0) {
                Mix_HaltMusic();
            }
            update();
            if (checkCollision()) {
                gameOver = true;
                gameState = GAME_OVER;
            }
            checkPowerUpCollection();
            draw();
        } else if (gameState == GAME_OVER) {
            if (Mix_PlayingMusic() == 0) {
                if (Mix_PlayMusic(gBackgroundMusic, -1) == -1) {
                    std::cerr << "Failed to play menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    exit(1);
                }
            }
            drawMenu(true);
            resetGame();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            handleMenuInput(e.key.keysym.sym);
            if (gameState == PLAYING) {
                player.handleInput(e.key.keysym.sym, bullets);
            }
        }
    }
}

bool isFarEnough(const SDL_Rect& rect1, const SDL_Rect& rect2, int minDistance) {
    int dx = rect1.x - rect2.x;
    int dy = rect1.y - rect2.y;
    int distance = sqrt(dx * dx + dy * dy);
    return distance > minDistance;
}

void Game::update() {
    if (gameOver) {
        return;
    }

    bgX -= bgSpeed;
    if (bgX <= -width) {
        bgX = 0;
    }

    player.update();
    //SDL_Rect adjustedPlayerRect = player.getRect();
    //adjustedPlayerRect.y -= 50;

    // Kiểm tra va chạm giữa viên đạn và quái vật
    for (size_t i = 0; i < bullets.size(); ++i) {
        for (size_t j = 0; j < enemies.size(); ++j) {
            SDL_Rect bulletRect = bullets[i].getRect();
            SDL_Rect enemyRect = enemies[j].getRect();

            if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);
                break;
            }
        }
    }

    // Kiểm tra va chạm giữa viên đạn và chướng ngại vật
    for (size_t i = 0; i < bullets.size(); ++i) {
        for (size_t j = 0; j < obstacles.size(); ++j) {
            SDL_Rect bulletRect = bullets[i].getRect();
            SDL_Rect obstacleRect = obstacles[j].getRect();

            if (SDL_HasIntersection(&bulletRect, &obstacleRect)) {
                bullets.erase(bullets.begin() + i);
                break;
            }
        }
    }

    // Cập nhật vị trí của quái vật
    for (auto& enemy : enemies) {
        enemy.update();
    }

    // Xóa các quái vật ra khỏi màn hình
    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
        SDL_Rect rect = enemy.getRect();
        return rect.x < -rect.w || rect.x > 1200 + rect.w || rect.y < -rect.h || rect.y > 700 + rect.h;
    }), enemies.end());

    // Cập nhật vị trí của các chướng ngại vật và vật phẩm
    for (auto& obstacle : obstacles) {
        obstacle.update();
    }

    if (rand() % 100 < 0) {  // Xác suất xuất hiện quái vật
        int side = rand() % 4; // Chọn ngẫu nhiên cạnh màn hình để xuất hiện quái vật
        int x, y, direction;
        int width = 50;
        int height = 50;
        SDL_Texture* enemyTexture;

        switch (side) {
            case 0: // Từ cạnh trái
                x = -width;
                y = rand() % (height - 100) + 100;
                direction = 1;  // Di chuyển sang phải
                break;
            case 1: // Từ cạnh phải
                x = 1200;
                y = rand() % (height - 100) + 100;
                direction = 0;  // Di chuyển sang trái

                break;
            case 2: // Từ cạnh trên
                x = rand() % 1200; // Phạm vi chiều ngang
                y = -height;
                direction = 2;  // Di chuyển xuống
                break;
            case 3:
                x = rand() % 1200;
                y = 700 - height;
                direction = 3;
                break;
        }

        SDL_Rect newEnemyRect = {x, y, width, height};
        bool canPlaceEnemy = true;
        int minDistance = 200;  // Khoảng cách tối thiểu giữa các quái vật

        for (const auto& enemy : enemies) {
            if (!isFarEnough(newEnemyRect, enemy.getRect(), minDistance)) {
                canPlaceEnemy = false;
                break;
            }
        }

        if (canPlaceEnemy) {
            if (y == groundHeight) {
                enemyTexture = gEnemyRunTexture;
            } else {
                enemyTexture = gEnemyFlyTexture;
            }

            enemies.push_back(Enemy(x, y, width, height, direction));
            enemies.back().setTexture(enemyTexture);  // Gán texture tương ứng cho quái vật
        }
    }

    if (rand() % 300 < 0) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        obstacles.push_back(Obstacle(width, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight));
    }

    if (rand() % 100 < 5) {
        int x = width;
        int y = height - groundHeight - powerUpHeight;
        PowerUpType type = static_cast<PowerUpType>(rand() % 3 + 1);
        PowerUp powerUp(x, y, powerUpWidth, powerUpHeight, type);
        powerUp.setTexture(powerUpHighJumpTexture, powerUpFlyTexture, powerUpInvincibleTexture);

        SDL_Rect newPowerupRect = {x, y, width, height};
        bool canPlacePowerup = true;
        int minDistance = 200;

        for (const auto& powerup : powerUps) {
            if (!isFarEnough(newPowerupRect, powerup.getRect(), minDistance)) {
                canPlacePowerup = false;
                break;
            }
        }

        if (canPlacePowerup) {
            powerUps.push_back(powerUp);
        }
    }

    for (auto& powerUp : powerUps) {
        powerUp.update();
    }

    for (auto& bullet : bullets) {
        bullet.update();
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
        return b.getRect().x > 1200;
    }), bullets.end());

    if (checkCollision()) {
        gameOver = true;
    }

    checkPowerUpCollection();

    if (powerUpDuration > 0) {
        powerUpDuration--;
        if (powerUpDuration == 0) {
            if (currentPowerUp == HIGH_JUMP) {
                player.setJumpHeight(120);
            } else if (currentPowerUp == FLY) {
                player.setFlying(false);
            }
            currentPowerUp = NONE;
        }
    }
}

void Game::draw() {
    //SDL_SetRenderDrawColor(renderer, 0, 0, 300, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect1 = { bgX, 0, width, height };
    SDL_Rect bgRect2 = { bgX + width, 0, width, height };

    SDL_RenderCopy(renderer, gBackgroundTexture, nullptr, &bgRect1);
    SDL_RenderCopy(renderer, gBackgroundTexture, nullptr, &bgRect2);

    bgX -= bgSpeed;
    if (bgX <= -width) {
        bgX = 0;
    }

    player.draw(renderer, gPlayerTexture);

    for (auto& enemy : enemies) {
        enemy.draw(renderer);
    }

    for (auto& obstacle : obstacles) {
        obstacle.draw(renderer, gObstacleTexture);
    }

    for (auto& powerUp : powerUps) {
        powerUp.draw(renderer);
    }

    for (auto& bullet : bullets) {
        bullet.draw(renderer, gBulletTexture);
    }

    SDL_RenderPresent(renderer);
}

void Game::drawMenu(bool isGameOverMenu) {
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = { 0, 0, width, height };
    SDL_RenderCopy(renderer, gMenuBackgroundTexture, nullptr, &bgRect);

    SDL_Color normalColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 0, 0, 255};

    const char* menuTitle = isGameOverMenu ? "Game Over" : "Conqueror's Journey";
    const char* playText = isGameOverMenu ? "Try again" : "Play";
    const char* exitText = "Exit";

    TTF_Font* font = TTF_OpenFont("asset/fonts/Atop-R99O3.ttf", 36);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, menuTitle, normalColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    int titleW, titleH;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleW, &titleH);
    SDL_Rect titleRect = { width / 2 - titleW / 2, height / 4 - titleH / 2, titleW, titleH };
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    SDL_Surface* playSurface = TTF_RenderText_Solid(font, playText, selectedMenuOption == 0 ? selectedColor : normalColor);
    SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
    int playW, playH;
    SDL_QueryTexture(playTexture, nullptr, nullptr, &playW, &playH);
    SDL_Rect playRect = { width / 2 - playW / 2, height / 2 - playH / 2, playW, playH };
    SDL_RenderCopy(renderer, playTexture, nullptr, &playRect);
    SDL_FreeSurface(playSurface);
    SDL_DestroyTexture(playTexture);

    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, exitText, selectedMenuOption == 1 ? selectedColor : normalColor);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    int exitW, exitH;
    SDL_QueryTexture(exitTexture, nullptr, nullptr, &exitW, &exitH);
    SDL_Rect exitRect = { width / 2 - exitW / 2, height / 2 + 50, exitW, exitH };
    SDL_RenderCopy(renderer, exitTexture, nullptr, &exitRect);
    SDL_FreeSurface(exitSurface);
    SDL_DestroyTexture(exitTexture);

    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    player.reset();
    obstacles.clear();
    powerUps.clear();
    enemies.clear();
    bullets.clear();
    player.setFlying(false);

    currentPowerUp = NONE;
    powerUpDuration = 0;
    gameOver = false;

    // Khởi tạo Enemy ban đầu
    for (int i = 0; i < 5; i++) {
        int x = rand() % 1200;
        int y = height - 140;
        int width = 20;
        int height = 40;
        int direction = rand() % 4;
        enemies.push_back(Enemy(x, y, width, height, direction));
    }
}

void Game::handleMenuInput(SDL_Keycode key) {
    if (gameState == MENU || gameState == GAME_OVER) {
        switch (key) {
            case SDLK_UP:
                selectedMenuOption = (selectedMenuOption - 1 + 2) % 2;
                break;
            case SDLK_DOWN:
                selectedMenuOption = (selectedMenuOption + 1) % 2;
                break;
            case SDLK_RETURN:
                if (selectedMenuOption == 0) {
                    resetGame();
                    gameState = PLAYING;
                } else {
                    quit = true;
                }
                break;
        }
    }
}

bool Game::checkCollision() {
    if (currentPowerUp == INVINCIBLE) {
        return false;
    }

    SDL_Rect playerRect = player.getRect();

    SDL_Rect adjustedPlayerRect = {
        playerRect.x + 10,
        playerRect.y + 10,
        playerRect.w - 20,
        playerRect.h - 20
    };

    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = obstacle.getRect();

        SDL_Rect adjustedObstacleRect = {
            obstacleRect.x + 5,
            obstacleRect.y + 5,
            obstacleRect.w - 10,
            obstacleRect.h - 10
        };

        if (SDL_HasIntersection(&adjustedPlayerRect, &adjustedObstacleRect)) {
            return true;
        }
    }

    for (const auto& enemy : enemies) {
        SDL_Rect enemyRect = enemy.getRect();

        SDL_Rect adjustedEnemyRect = {
            enemyRect.x + 5,
            enemyRect.y + 5,
            enemyRect.w - 10,
            enemyRect.h - 10
        };

        if (SDL_HasIntersection(&adjustedPlayerRect, &adjustedEnemyRect)) {
            return true;
        }
    }

    return false;
}


bool Game::checkPowerUpCollection() {
    SDL_Rect playerRect = player.getRect();
    for (size_t i = 0; i < powerUps.size(); i++) {
        SDL_Rect powerUpRect = powerUps[i].getRect();
        if (SDL_HasIntersection(&playerRect, &powerUpRect)) {
            currentPowerUp = powerUps[i].getType();

            if (currentPowerUp == INVINCIBLE) {
                powerUpDuration = 600;
                player.setFlying(false);
            } else if (currentPowerUp == HIGH_JUMP) {
                powerUpDuration = 300;
                player.setJumpHeight(240);
                player.setFlying(false);
            } else if (currentPowerUp == FLY) {
                powerUpDuration = 600;
                player.setFlying(true);
            }

            powerUps.erase(powerUps.begin() + i);
            return true;
        }
    }
    return false;
}

