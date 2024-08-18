#include "Game.h"
#include "Player.h"
#include "Enemy.h"

#include <SDL_ttf.h>

#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <string>

using namespace std;

Mix_Music* gBackgroundMusic = nullptr;
Mix_Music* gPlayMusic = nullptr;
Mix_Chunk* gJumpSound = nullptr;
Mix_Chunk* gShootSound = nullptr;
Mix_Chunk* gEnemySpawnSound = nullptr;
Mix_Chunk* gItemCollectSound = nullptr;
Mix_Chunk* gPowerUpCollectSound = nullptr;
Mix_Chunk* gCollisionSound = nullptr;
Mix_Chunk* gCollisionObsSound = nullptr;
Mix_Chunk* gWinSound = nullptr;

Game::Game() : width(1200), height(700), window(nullptr), renderer(nullptr), gameState(MENU), selectedMenuOption(0),
               selectedGameOverOption(0), quit(false), gameOver(false), currentPowerUp(NONE), powerUpDuration(0), player(width, height),
               bgX(0), bgSpeed(2), mapCount(0) {
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

    SDL_Surface* iconSurface = IMG_Load("asset/img/logo.jfif");
    if (iconSurface == nullptr) {
        std::cerr << "Failed to load icon image! SDL_image Error: " << IMG_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("Conqueror's Journey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);

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

    gBulletTexture = loadTexture("asset/img/bullet.png");
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

    gEnemyRunTexture = loadTexture("asset/img/enemy-run.png");
    if (gEnemyRunTexture == nullptr) {
        cerr << "Failed to load enemy image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Enemy image loaded successfully!" << endl;
    }

    gEnemyFlyTexture = loadTexture("asset/img/enemy-fly.png");
    if (gEnemyFlyTexture == nullptr) {
        cerr << "Failed to load enemy image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Enemy image loaded successfully!" << endl;
    }

    gBackgroundTexture = loadTexture("asset/img/bg.jpg");
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

    gAxeTexture = loadTexture("asset/img/axe.png");
    gMapTexture = loadTexture("asset/img/map.png");
    gDrumstickTexture = loadTexture("asset/img/bread.png");
    gBagTexture = loadTexture("asset/img/bag.png");
    if (!gAxeTexture || !gMapTexture || !gDrumstickTexture || !gBagTexture) {
        std::cerr << "Failed to load items images!" << std::endl;
        exit(1);
    } else {
        cerr << "Items image loaded successfully!" << endl;
    }

    gMenuBackgroundTexture = loadTexture("asset/img/menu-bg.png");
    if (!gMenuBackgroundTexture) {
        std::cerr << "Failed to load menu bg images!" << std::endl;
        exit(1);
    } else {
        cerr << "Menu bg image loaded successfully!" << endl;
    }

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

    gPlayMusic = Mix_LoadMUS("asset/sound/bg-sound.mp3");
    if (gBackgroundMusic == nullptr) {
        std::cerr << "Failed to load play music! SDL_mixer Error: " << Mix_GetError() << std::endl;
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

    gEnemySpawnSound = Mix_LoadWAV("asset/sound/enemy.wav");
    if (gEnemySpawnSound == nullptr) {
        std::cerr << "Failed to load enemy spawn sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gItemCollectSound = Mix_LoadWAV("asset/sound/item.wav");
    if (gItemCollectSound == nullptr) {
        std::cerr << "Failed to load item collect sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gPowerUpCollectSound = Mix_LoadWAV("asset/sound/powerup.wav");
    if (gPowerUpCollectSound == nullptr) {
        std::cerr << "Failed to load powerup collect sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gCollisionSound = Mix_LoadWAV("asset/sound/player-die.mp3");
    if (gCollisionSound == nullptr) {
        std::cerr << "Failed to load collision sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gCollisionObsSound = Mix_LoadWAV("asset/sound/obstacle.wav");
    if (gCollisionObsSound == nullptr) {
        std::cerr << "Failed to load collision obstacle sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    gWinSound = Mix_LoadWAV("asset/sound/win.wav");
    if (gWinSound == nullptr) {
        std::cerr << "Failed to load collision obstacle sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
        exit(1);
    }

    //Font
}


void Game::closeSDL() {
    SDL_DestroyTexture(gBackgroundTexture);
    gBackgroundTexture = nullptr;

    SDL_DestroyTexture(gMenuBackgroundTexture);
    gMenuBackgroundTexture = nullptr;

    SDL_DestroyTexture(gPlayerTexture);
    gPlayerTexture = nullptr;

    SDL_DestroyTexture(gBulletTexture);
    gBulletTexture = nullptr;

    SDL_DestroyTexture(gObstacleTexture);
    gObstacleTexture = nullptr;

    SDL_DestroyTexture(gEnemyRunLeftTexture);
    gEnemyRunLeftTexture = nullptr;

    SDL_DestroyTexture(gEnemyRunRightTexture);
    gEnemyRunRightTexture = nullptr;

    SDL_DestroyTexture(gEnemyFlyUpTexture);
    gEnemyFlyUpTexture = nullptr;

    SDL_DestroyTexture(gEnemyFlyDownTexture);
    gEnemyFlyDownTexture = nullptr;

    SDL_DestroyTexture(powerUpHighJumpTexture);
    powerUpHighJumpTexture = nullptr;

    SDL_DestroyTexture(powerUpFlyTexture);
    powerUpFlyTexture = nullptr;

    SDL_DestroyTexture(powerUpInvincibleTexture);
    powerUpInvincibleTexture = nullptr;

    SDL_DestroyTexture(gDrumstickTexture);
    gDrumstickTexture = nullptr;

    SDL_DestroyTexture(gAxeTexture);
    gAxeTexture = nullptr;

    SDL_DestroyTexture(gMapTexture);
    gMapTexture = nullptr;

    SDL_DestroyTexture(gBagTexture);
    gBagTexture = nullptr;

    //Sound
    Mix_FreeMusic(gBackgroundMusic);
    gBackgroundMusic = nullptr;

    Mix_FreeMusic(gPlayMusic);
    gPlayMusic = nullptr;

    Mix_FreeChunk(gJumpSound);
    gJumpSound = nullptr;

    Mix_FreeChunk(gShootSound);
    gShootSound = nullptr;

    Mix_FreeChunk(gEnemySpawnSound);
    gEnemySpawnSound = nullptr;

    Mix_FreeChunk(gItemCollectSound);
    gItemCollectSound = nullptr;

    Mix_FreeChunk(gPowerUpCollectSound);
    gPowerUpCollectSound = nullptr;

    Mix_FreeChunk(gCollisionSound);
    gCollisionSound = nullptr;

    Mix_FreeChunk(gCollisionObsSound);
    gCollisionObsSound = nullptr;

    Mix_CloseAudio();

    //TTF_CloseFont(gStaminaFont);
    //gStaminaFont = nullptr;

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    while (!quit) {
        handleEvents();
        if (gameState == MENU) {
            if (Mix_PlayingMusic() == 0) {
                if (Mix_PlayMusic(gBackgroundMusic, -1) == -1) {
                    std::cerr << "Failed to play menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    exit(1);
                }
            }
            drawMenu();
        } else if (gameState == PLAYING) {
            Mix_HaltMusic();
            update();
            if (checkCollision()) {
                gameOver = true;
                gameState = GAME_OVER;
            }
            checkPowerUpCollection();
            draw();
        } else if (gameState == GAME_OVER) {
            Mix_HaltMusic();
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

    for (size_t i = 0; i < bullets.size(); ++i) {
        for (size_t j = 0; j < enemies.size(); ++j) {
            SDL_Rect bulletRect = bullets[i].getRect();
            SDL_Rect enemyRect = enemies[j].getRect();

            if (SDL_HasIntersection(&bulletRect, &enemyRect)) {
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + j);
                updateScore();
                break;
            }
        }
    }

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

    for (size_t i = 0; i < items.size(); i++) {
        SDL_Rect playerRect = player.getRect();
        SDL_Rect itemRect = items[i].getRect();

        if (SDL_HasIntersection(&playerRect, &itemRect)) {
            ItemType type = items[i].getType();

            if (type == AXE) {
                player.collectAxe(true);
            } else if (type == MAP) {
                score += 30;
                mapCount++;
            } else if (type == DRUMSTICK) {
                player.increaseStamina(10);
            } else if (type == BULLET) {
                player.increaseBullets(5);
            }

            // Xóa item sau khi thu thập
            items.erase(items.begin() + i);
            Mix_PlayChannel(-1, gItemCollectSound, 0);
        }
    }

    for (auto& enemy : enemies) {
        enemy.update();
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
        SDL_Rect rect = enemy.getRect();
        return rect.x < -rect.w || rect.x > 1200 + rect.w || rect.y < -rect.h || rect.y > 700 + rect.h;
    }), enemies.end());

    for (auto& obstacle : obstacles) {
        obstacle.update();
    }

    SDL_Rect playerRect = player.getRect();
    for (size_t i = 0; i < obstacles.size(); ++i) {
        SDL_Rect obstacleRect = obstacles[i].getRect();

        if (SDL_HasIntersection(&playerRect, &obstacleRect)) {
            if (player.canBreakObstacle()) {
                obstacles.erase(obstacles.begin() + i);
                Mix_PlayChannel(-1, gCollisionObsSound, 0);
                Mix_Volume(0, 50);
                player.collectAxe(false);
                break;
            }
        }
    }

    if (rand() % 100 < 1) {
        int x = width;
        int y = rand() % (height - 250);

        if (rand() % 100 < 60) {
            y = height - 230;
        } else {
            y = rand() % (height - 300);
        }

        int enemyWidth, enemyHeight;

        bool isRunning = (y == height - 230);

        if (isRunning) {
            enemyWidth = 85;
            enemyHeight = 90;
        } else {
            enemyWidth = 105;
            enemyHeight = 80;
        }

        SDL_Rect newEnemyRect = {x, y, enemyWidth, enemyHeight};
        bool canPlaceEnemy = true;
        int minDistance = 100;

        for (const auto& enemy : enemies) {
            SDL_Rect existingEnemyRect = enemy.getRect();
            int dx = newEnemyRect.x - existingEnemyRect.x;
            int dy = newEnemyRect.y - existingEnemyRect.y;
            int distance = sqrt(dx * dx + dy * dy);
            if (distance < minDistance || SDL_HasIntersection(&newEnemyRect, &existingEnemyRect)) {
                canPlaceEnemy = false;
                break;
            }
        }

        if (canPlaceEnemy) {
            Enemy newEnemy(x, y, enemyWidth, enemyHeight, groundHeight, isRunning);

            if (isRunning) {
                newEnemy.setTexture(gEnemyRunTexture);
            } else {
                newEnemy.setTexture(gEnemyFlyTexture);
            }

            enemies.push_back(newEnemy);
            Mix_PlayChannel(-1, gEnemySpawnSound, 0);
            Mix_VolumeMusic(5);
        }
    }

    if (rand() % 300 < 1) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        int obstacleWidth = minObstacleWidth + rand() % (maxObstacleWidth - minObstacleWidth + 1);
        obstacles.push_back(Obstacle(width, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight));
    }

    if (rand() % 100 < 1) {
        int minX = player.getRect().x + player.getRect().w + 120;
        int x = minX + rand() % (width - minX);
        int y = rand() % ((height - 250) / 3);

        SDL_Rect newItemRect = {x, y, 40, 40};

        bool canPlaceItem = true;
        int minDistance = 200;

        for (const auto& item : items) {
            SDL_Rect existingItemRect = item.getRect();
            int dx = newItemRect.x - existingItemRect.x;
            int dy = newItemRect.y - existingItemRect.y;
            int distance = sqrt(dx * dx + dy * dy);

            if (distance < minDistance || SDL_HasIntersection(&newItemRect, &existingItemRect)) {
                canPlaceItem = false;
                break;
            }
        }

        for (const auto& obstacle : obstacles) {
            SDL_Rect obstacleRect = obstacle.getRect();
            if (SDL_HasIntersection(&newItemRect, &obstacleRect)) {
                canPlaceItem = false;
                break;
            }
        }

        if (canPlaceItem) {
            ItemType type;
            int randType = rand() % 4;

            if (randType == 0) {
                type = AXE;
            } else if (randType == 1) {
                type = MAP;
            } else if (randType == 2) {
                type = DRUMSTICK;
            } else {
                type = BULLET;
            }

            Item newItem(x, y, 40, 40, type);

            items.push_back(newItem);
        }
    }

    if (rand() % 100 < 1) {
        int x = width;
        int y = height - groundHeight - powerUpHeight - 10;
        PowerUpType type = static_cast<PowerUpType>(rand() % 3);
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

        for (const auto& obstacle : obstacles) {
            SDL_Rect obstacleRect = obstacle.getRect();
            if (SDL_HasIntersection(&newPowerupRect, &obstacleRect) || !isFarEnough(newPowerupRect, obstacleRect, 100)) {
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
        cerr << "Collision detected, setting gameOver to true." << std::endl;
        Mix_PlayChannel(-1, gCollisionSound, 0);
        gameOver = true;
    }

    if (powerUpDuration > 0) {
        checkPowerUpCollection();
        powerUpDuration--;
        if (powerUpDuration <= 0) {
            if (currentPowerUp == HIGH_JUMP) {
                player.setJumpHeight(120);
            } else if (currentPowerUp == FLY) {
                player.setFlying(false);
            }
            currentPowerUp = NONE;
        }
    }
}

void Game::loadHighScore() {
    std::ifstream file(highScoreFile);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        cerr << "Failed to load score from this file !!!" << endl;
        highScore = 0;
    }
}

void Game::saveHighScore() {
    std::ofstream file(highScoreFile);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Game::updateScore() {
    score += 5;
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }

     if (mapCount >= 10) {
        gameState = GAME_OVER;
        Mix_HaltMusic();
        Mix_PlayChannel(-1, gWinSound, 0);
    }
}

void Game::draw() {
    TTF_Font* gStaminaFont = TTF_OpenFont("asset/fonts/Atop-R99O3.ttf", 24);
    if (gStaminaFont == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

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

    player.draw(renderer, gPlayerTexture, gStaminaFont);

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

    for (auto& item : items) {
        if (item.getType() == AXE) {
            item.draw(renderer, gAxeTexture);
        } else if (item.getType() == MAP) {
            item.draw(renderer, gMapTexture);
        } else if (item.getType() == DRUMSTICK) {
            item.draw(renderer, gDrumstickTexture);
        } else if (item.getType() == BULLET) {
            item.draw(renderer, gBagTexture);
        }
    }

    drawScore();

    SDL_RenderPresent(renderer);

    TTF_CloseFont(gStaminaFont);
}

void Game::drawScore() {
    TTF_Font* font = TTF_OpenFont("asset/fonts/Atop-R99O3.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    SDL_Color color = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), color);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    int scoreW, scoreH;
    SDL_QueryTexture(scoreTexture, nullptr, nullptr, &scoreW, &scoreH);
    SDL_Rect scoreRect = { width - scoreW - 20, 20, scoreW, scoreH };
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    // Hiển thị số lượng đạn
    std::string bulletText = "Bullets: " + std::to_string(player.getBulletCount());
    SDL_Surface* bulletSurface = TTF_RenderText_Solid(font, bulletText.c_str(), color);
    SDL_Texture* bulletTexture = SDL_CreateTextureFromSurface(renderer, bulletSurface);
    int bulletW, bulletH;
    SDL_QueryTexture(bulletTexture, nullptr, nullptr, &bulletW, &bulletH);
    SDL_Rect bulletRect = { width - bulletW - 20, 60, bulletW, bulletH };
    SDL_RenderCopy(renderer, bulletTexture, nullptr, &bulletRect);
    SDL_FreeSurface(bulletSurface);
    SDL_DestroyTexture(bulletTexture);

    //Hiển thị số lượng Map
    std::string mapText = "Maps: " + std::to_string(mapCount) + " / 10";
    SDL_Surface* mapSurface = TTF_RenderText_Solid(font, mapText.c_str(), color);
    SDL_Texture* mapTexture = SDL_CreateTextureFromSurface(renderer, mapSurface);
    int mapW, mapH;
    SDL_QueryTexture(mapTexture, nullptr, nullptr, &mapW, &mapH);
    SDL_Rect mapRect = { width - mapW - 20, 100, mapW, mapH };
    SDL_RenderCopy(renderer, mapTexture, nullptr, &mapRect);
    SDL_FreeSurface(mapSurface);
    SDL_DestroyTexture(mapTexture);

    TTF_CloseFont(font);
}

void Game::drawMenu(bool isGameOverMenu) {
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = { 0, 0, width, height };
    SDL_RenderCopy(renderer, gMenuBackgroundTexture, nullptr, &bgRect);

    // Adjusting colors for better contrast
    SDL_Color normalColor = {128, 128, 128, 255}; // White color for normal options
    SDL_Color selectedColor = {0, 155, 0, 255};  // Green color for the selected option
    SDL_Color titleColor = {0, 0, 155, 255};     // Blue color for the title

    const char* menuTitle = isGameOverMenu ? "Game Over" : "Conqueror's Journey";
    const char* playText = isGameOverMenu ? "Try again" : "Play";
    const char* exitText = "Exit";

    TTF_Font* font = TTF_OpenFont("asset/fonts/Atop-R99O3.ttf", 36);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        exit(1);
    }

    // Render the title
    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, menuTitle, titleColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    int titleW, titleH;
    SDL_QueryTexture(titleTexture, nullptr, nullptr, &titleW, &titleH);
    SDL_Rect titleRect = { width / 2 - titleW / 2, height / 4 - titleH / 2, titleW, titleH };
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    // Render the "Play" option
    SDL_Surface* playSurface = TTF_RenderText_Solid(font, playText, selectedMenuOption == 0 ? selectedColor : normalColor);
    SDL_Texture* playTexture = SDL_CreateTextureFromSurface(renderer, playSurface);
    int playW, playH;
    SDL_QueryTexture(playTexture, nullptr, nullptr, &playW, &playH);
    SDL_Rect playRect = { width / 2 - playW / 2, height / 2 - playH / 2, playW, playH };
    SDL_RenderCopy(renderer, playTexture, nullptr, &playRect);
    SDL_FreeSurface(playSurface);
    SDL_DestroyTexture(playTexture);

    // Render the "Exit" option
    SDL_Surface* exitSurface = TTF_RenderText_Solid(font, exitText, selectedMenuOption == 1 ? selectedColor : normalColor);
    SDL_Texture* exitTexture = SDL_CreateTextureFromSurface(renderer, exitSurface);
    int exitW, exitH;
    SDL_QueryTexture(exitTexture, nullptr, nullptr, &exitW, &exitH);
    SDL_Rect exitRect = { width / 2 - exitW / 2, height / 2 + 30, exitW, exitH };
    SDL_RenderCopy(renderer, exitTexture, nullptr, &exitRect);
    SDL_FreeSurface(exitSurface);
    SDL_DestroyTexture(exitTexture);

    // Render the high score
    std::string highScoreText = "High Score: " + std::to_string(highScore);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreText.c_str(), normalColor);
    SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(renderer, highScoreSurface);
    int highScoreW, highScoreH;
    SDL_QueryTexture(highScoreTexture, nullptr, nullptr, &highScoreW, &highScoreH);
    SDL_Rect highScoreRect = { width / 2 - highScoreW / 2, height / 2 + 80, highScoreW, highScoreH };
    SDL_RenderCopy(renderer, highScoreTexture, nullptr, &highScoreRect);
    SDL_FreeSurface(highScoreSurface);
    SDL_DestroyTexture(highScoreTexture);

    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    score = 0;
    mapCount = 0;
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
        playerRect.x,
        playerRect.y,
        playerRect.w - 20,
        playerRect.h - 20
    };

    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = obstacle.getRect();

        SDL_Rect adjustedObstacleRect = {
            obstacleRect.x,
            obstacleRect.y,
            obstacleRect.w - 10,
            obstacleRect.h - 10
        };

        if (SDL_HasIntersection(&adjustedPlayerRect, &adjustedObstacleRect) && player.canBreakObstacle() == false) {
            return true;
            std::cerr << "Player position: (" << playerRect.x << ", " << playerRect.y << ")" << std::endl;
            std::cerr << "Game over triggered due to collision: Player vs Obstacle" << std::endl;
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
            cerr << "Player position: (" << playerRect.x << ", " << playerRect.y << ")" << endl;
            cerr << "Game over triggered due to collision: Player vs Enemy" << endl;

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
                player.setJumpHeight(180);
                player.setFlying(false);
            } else if (currentPowerUp == FLY) {
                powerUpDuration = 600;
                player.setFlying(true);
            }

            powerUps.erase(powerUps.begin() + i);
            Mix_PlayChannel(-1, gPowerUpCollectSound, 0);
            return true;
        }
    }
    return false;
}

