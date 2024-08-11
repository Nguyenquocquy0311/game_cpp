#include "Game.h"
#include "Player.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <algorithm>

Game::Game() : width(1200), height(600), window(nullptr), renderer(nullptr), gameState(MENU), selectedMenuOption(0),
               selectedGameOverOption(0), quit(false), gameOver(false), currentPowerUp(NONE), powerUpDuration(0), player(width, height) {
    initSDL();
}

Game::~Game() {
    closeSDL();
}

void Game::initSDL() {
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
}

void Game::closeSDL() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    while (!quit) {
        handleEvents();
        if (gameState == MENU) {
            drawMenu();
        } else if (gameState == PLAYING) {
            update();
            if (checkCollision()) {
                gameOver = true;
                gameState = GAME_OVER;
            }
            checkPowerUpCollection();
            draw();
        } else if (gameState == GAME_OVER) {
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

    for (auto& enemy : enemies) {
        enemy.update();
    }

    // Xóa những enemy đã ra khỏi màn hình
    enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Enemy& enemy) {
        SDL_Rect rect = enemy.getRect();
        return rect.x < 0 || rect.x > 1200 || rect.y < 0 || rect.y > 600;
    }), enemies.end());

    for (auto& obstacle : obstacles) {
        obstacle.update();
    }

    for (auto& powerUp : powerUps) {
        powerUp.update();
    }

    // Tạo mới Enemy
    if (rand() % 500 < 5) {
        int x = (rand() % 2 == 0) ? 0 : 1200 - 20;
        int y = rand() % 400 + 200;
        int width = 20;
        int height = 40;
        int direction = rand() % 4;
        enemies.push_back(Enemy(x, y, width, height, direction));
    }

    if (rand() % 300 < 7) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        obstacles.push_back(Obstacle(width, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight));
    }

    if (rand() % 1000 < 10) {
        powerUps.push_back(PowerUp(rand() % width, height - groundHeight - powerUpHeight, powerUpWidth, powerUpHeight, static_cast<PowerUpType>(rand() % 3 + 1)));
    }

    // Cập nhật đạn
    for (auto& bullet : bullets) {
        bullet.update();
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) {
        return b.getRect().x > 1200;
    }), bullets.end());

    if (powerUpDuration > 0) {
        powerUpDuration--;
        if (powerUpDuration == 0) {
            // Hết thời gian hiệu lực thì reset lại các thuộc tính của người chơi
            if (currentPowerUp == HIGH_JUMP) {
                player.setJumpHeight(120);  // Đặt lại nhảy cao về bình thường
            } else if (currentPowerUp == LONG_JUMP) {
                player.setJumpSpeed(10);  // Đặt lại tốc độ nhảy về bình thường
            }
            currentPowerUp = NONE;  // Reset vật phẩm hiện tại
        }
    }

    // Kiểm tra va chạm giữa người chơi và vật cản, vật phẩm
    if (checkCollision()) {
        gameOver = true;
    }

    // Kiểm tra thu thập vật phẩm
    checkPowerUpCollection();
}

void Game::draw() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ mặt đất
    SDL_Rect groundRect = { 0, height - 200, width, 200 };
    SDL_SetRenderDrawColor(renderer, 0, 355, 0, 255);
    SDL_RenderFillRect(renderer, &groundRect);

    // Vẽ người chơi
    player.draw(renderer);

    // Vẽ quái vật
    for (const auto& enemy : enemies) {
        enemy.draw(renderer);
    }

    // Vẽ vật cản
    for (auto& obstacle : obstacles) {

        obstacle.draw(renderer);
    }

    // Vẽ vật phẩm
    for (auto& powerUp : powerUps) {

        powerUp.draw(renderer);
    }

    // Vẽ đạn
    for (const auto& bullet : bullets) {
        bullet.draw(renderer);
    }

    SDL_RenderPresent(renderer);
}


void Game::drawMenu(bool isGameOverMenu) {
    SDL_SetRenderDrawColor(renderer, isGameOverMenu ? 50 : 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 200, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(textSurface, nullptr, SDL_MapRGB(textSurface->format, 255, 255, 255));
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect startGameRect = { 1200 / 2 - 100, 600 / 2 - 50, 200, 50 };
    SDL_Rect exitGameRect = { 1200 / 2 - 100, 600 / 2 + 10, 200, 50 };

    if (selectedMenuOption == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect borderRect = { startGameRect.x - 5, startGameRect.y - 5, startGameRect.w + 10, startGameRect.h + 10 };
        SDL_RenderFillRect(renderer, &borderRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_RenderFillRect(renderer, &startGameRect);

    if (selectedMenuOption == 1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect borderRect = { exitGameRect.x - 5, exitGameRect.y - 5, exitGameRect.w + 10, exitGameRect.h + 10 };
        SDL_RenderFillRect(renderer, &borderRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    SDL_RenderFillRect(renderer, &exitGameRect);

    SDL_RenderCopy(renderer, textTexture, NULL, &startGameRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &exitGameRect);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}

void Game::resetGame() {
    player.reset();
    obstacles.clear();
    powerUps.clear();
    enemies.clear();
    bullets.clear();

    currentPowerUp = NONE;
    powerUpDuration = 0;
    gameOver = false;

    // Khởi tạo Enemy ban đầu
    for (int i = 0; i < 5; i++) {
        int x = rand() % 1200;
        int y = rand() % 400 + 200;
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
            } else if (currentPowerUp == LONG_JUMP) {
                powerUpDuration = 300;  // Ví dụ: 5 giây cho nhảy xa
                player.setJumpSpeed(20);
            } else {
                powerUpDuration = 300;  // Thời gian cho các loại vật phẩm khác
            }

            powerUps.erase(powerUps.begin() + i);
            return true;
        }
    }
    return false;
}

