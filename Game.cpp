#include "Game.h"
#include "Player.h"
#include "Enemy.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>

using namespace std;

SDL_Texture* gPlayerTexture = nullptr;

Mix_Music* gBackgroundMusic = nullptr;
Mix_Chunk* gJumpSound = nullptr;
Mix_Chunk* gShootSound = nullptr;


Game::Game() : width(1200), height(700), window(nullptr), renderer(nullptr), gameState(MENU), selectedMenuOption(0),
               selectedGameOverOption(0), quit(false), gameOver(false), currentPowerUp(NONE), powerUpDuration(0), player(width, height),
               bgX(0), bgSpeed(3) {
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

    gPlayerTexture = loadTexture("asset/img/person.png");
    if (gPlayerTexture == nullptr) {
        cerr << "Failed to load player image! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    } else {
        cerr << "Player image loaded successfully!" << endl;
    }

    playerRect.x = 100;
    playerRect.y = height - 80 - playerRect.h;
    playerRect.w = 100;
    playerRect.h = 100;

    gBulletTexture = loadTexture("asset/img/bullet1.png");
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
    }

    gBackgroundTexture = loadTexture("asset/img/bg.jpg");
    if (gBackgroundTexture == nullptr) {
        std::cerr << "Failed to load background image! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    powerUpHighJumpTexture = loadTexture("asset/img/power-up-1.png");
    powerUpFlyTexture = loadTexture("asset/img/power-up-3.png");
    powerUpInvincibleTexture = loadTexture("asset/img/power-up-2.jfif");

    if (!powerUpHighJumpTexture || !powerUpFlyTexture || !powerUpInvincibleTexture) {
        std::cerr << "Failed to load power-up images!" << std::endl;
        exit(1);
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
            if (Mix_PlayingMusic() != 0) {  // Dừng nhạc nền khi vào game
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
            if (Mix_PlayingMusic() == 0) {  // Phát lại nhạc nếu quay lại menu
                if (Mix_PlayMusic(gBackgroundMusic, -1) == -1) {
                    std::cerr << "Failed to play menu music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    exit(1);
                }
            }
            drawMenu(true);
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

    for (auto& enemy : enemies) {
        enemy.update();
    }

    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
        SDL_Rect rect = enemy.getRect();
        return rect.x < 0 || rect.x > 1200 || rect.y < 0 || rect.y > 300;
    }), enemies.end());

    for (auto& obstacle : obstacles) {
        obstacle.update();
    }

    for (auto& powerUp : powerUps) {
        powerUp.update();
    }

    if (rand() % 500 < 5) {
        int x = (rand() % 2 == 0) ? 0 : 1200 - 20;
        int y = height - 140;
        int width = 20;
        int height = 40;
        int direction = rand() % 4;
        enemies.push_back(Enemy(x, y, width, height, direction));
    }

    if (rand() % 300 < 5) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        obstacles.push_back(Obstacle(width, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight));
    }

    if (rand() % 1000 < 10) {
        PowerUp powerUp(rand() % width, height - groundHeight - powerUpHeight, powerUpWidth, powerUpHeight, static_cast<PowerUpType>(rand() % 3 + 1));
        powerUp.loadTextures(renderer, "asset/img/power-up-1.png", "asset/img/power-up-2.jfif", "asset/img/power-up-3.png");
        powerUps.push_back(powerUp);
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
            }
            currentPowerUp = NONE;
        }
    }
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 300, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = { 0, 0, width, height };
    SDL_RenderCopy(renderer, gBackgroundTexture, nullptr, &bgRect);

    SDL_Rect bgRect1 = { bgX, 0, width, height };
    SDL_Rect bgRect2 = { bgX + width - 20, 0, width, height };
    SDL_RenderCopy(renderer, gBackgroundTexture, nullptr, &bgRect1);
    SDL_RenderCopy(renderer, gBackgroundTexture, nullptr, &bgRect2);

    player.draw(renderer, gPlayerTexture);

    for (const auto& enemy : enemies) {
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect bgRect = { 0, 0, width, height };
    SDL_RenderCopy(renderer, gMenuBackgroundTexture, NULL, &bgRect);

    SDL_Rect playButtonRect = { width / 2 - 150, height / 2 - 100, 300, 200 };
    SDL_RenderCopy(renderer, gPlayButtonTexture, NULL, &playButtonRect);

    SDL_Rect exitButtonRect = { width / 2 - 150, height / 2, 300, 200 };
    SDL_RenderCopy(renderer, gExitButtonTexture, NULL, &exitButtonRect);

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
        //EnemyType type = (rand() % 2 == 0) ? FLYING : RUNNING;
        //enemies.push_back(Enemy(x, y, width, height, direction, type));
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
    for (const auto& obstacle : obstacles) {
        SDL_Rect obstacleRect = obstacle.getRect(); // Tạo biến tạm thời
        if (SDL_HasIntersection(&playerRect, &obstacleRect)) {
            return true;
        }
    }
    for (const auto& enemy : enemies) {
        SDL_Rect enemyRect = enemy.getRect(); // Tạo biến tạm thời
        if (SDL_HasIntersection(&playerRect, &enemyRect)) {
            return true;
        }
    }
    return false;
}


bool Game::checkPowerUpCollection() {
    SDL_Rect playerRect = player.getRect();
    for (size_t i = 0; i < powerUps.size(); i++) {
        SDL_Rect powerUpRect = powerUps[i].getRect(); // Tạo biến tạm thời
        if (SDL_HasIntersection(&playerRect, &powerUpRect)) {
            currentPowerUp = powerUps[i].getType();

            if (currentPowerUp == INVINCIBLE) {
                powerUpDuration = 600;  // 10 giây cho bất tử
            } else if (currentPowerUp == HIGH_JUMP) {
                powerUpDuration = 300;  // 5 giây cho nhảy cao
                player.setJumpHeight(240);  // Nhảy cao gấp đôi
            } else if (currentPowerUp == FLY) {
                powerUpDuration = 600;  // 10 giây cho bay
                player.setFlying(true);  // Bật trạng thái bay
            } else {
                powerUpDuration = 300;  // Thời gian cho các loại vật phẩm khác
            }

            powerUps.erase(powerUps.begin() + i);
            return true;
        }
    }
    return false;
}
