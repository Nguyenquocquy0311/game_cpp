#include <SDL.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

const int width = 1200;
const int height = 600;
const int playerWidth = 20;
const int playerHeight = 40;
const int playerPosY = height - playerHeight - 200;
const int playerPosX = 50;
const int jumpHeight = 120;
const int obstacleWidth = 20;
const int powerUpWidth = 20;
const int powerUpHeight = 20;
const int groundHeight = 200;
const int minObstacleHeight = 20;
const int maxObstacleHeight = 100;

enum PowerUpType { NONE, HIGH_JUMP, LONG_JUMP, INVINCIBLE };
enum GameState { MENU, PLAYING, GAME_OVER };

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Rect playerRect = { playerPosX, playerPosY, playerWidth, playerHeight };
vector<SDL_Rect> obstacleRects;
vector<SDL_Rect> powerUpRects;
vector<SDL_Rect> enemyRects;
vector<PowerUpType> powerUpTypes;
bool isJumping = false;
bool isFalling = false;
bool gameOver = false;
int jumpSpeed = 8;
int fallSpeed = 8;
PowerUpType currentPowerUp = NONE;
int powerUpDuration = 0;
GameState gameState = MENU;
int selectedMenuOption = 0;

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    }
    window = SDL_CreateWindow("Conqueror's Journey", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        cerr << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
        exit(1);
    }
}

void closeSDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawMenu() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = { 255, 255, 255 };
    SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 200, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(textSurface, nullptr, SDL_MapRGB(textSurface->format, 255, 255, 255));
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);

    SDL_Rect startGameRect = { width / 2 - 100, height / 2 - 50, 200, 50 };
    SDL_Rect exitGameRect = { width / 2 - 100, height / 2 + 10, 200, 50 };

    if (selectedMenuOption == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &startGameRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &startGameRect);
    }

    if (selectedMenuOption == 1) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &exitGameRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &exitGameRect);
    }

    SDL_RenderCopy(renderer, textTexture, NULL, &startGameRect);
    SDL_RenderCopy(renderer, textTexture, NULL, &exitGameRect);
    SDL_DestroyTexture(textTexture);

    SDL_RenderPresent(renderer);
}

void draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Rect groundRect = { 0, height - groundHeight, width, groundHeight };
    SDL_SetRenderDrawColor(renderer, 0, 355, 0, 255);
    SDL_RenderFillRect(renderer, &groundRect);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerRect);

    // Draw static obstacles
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (const auto& rect : obstacleRects) {
        SDL_RenderFillRect(renderer, &rect);
    }

    // Draw power-ups
    for (int i = 0; i < (int)powerUpRects.size(); i++) {
        switch (powerUpTypes[i]) {
            case HIGH_JUMP:
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow for HIGH_JUMP
                break;
            case LONG_JUMP:
                SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Cyan for LONG_JUMP
                break;
            case INVINCIBLE:
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // Magenta for INVINCIBLE
                break;
            default:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for unknown (should not happen)
                break;
        }
        SDL_RenderFillRect(renderer, &powerUpRects[i]);
    }

    // Draw moving enemies
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto& rect : enemyRects) {
        SDL_RenderFillRect(renderer, &rect);
    }

    if (gameOver) {
        SDL_Color textColor = { 255, 0, 0 };
        SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 200, 100, 32, 0, 0, 0, 0);
        SDL_FillRect(textSurface, nullptr, SDL_MapRGB(textSurface->format, 255, 0, 0));
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_Rect textRect = { width / 2 - 100, height / 2 - 50, 200, 100 };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
        SDL_DestroyTexture(textTexture);
    }

    SDL_RenderPresent(renderer);
}

void update() {
    if (gameOver) {
        return;
    }

    for (auto& rect : obstacleRects) {
        rect.x -= 5;
    }

    if (!obstacleRects.empty() && obstacleRects[0].x + obstacleWidth < 0) {
        obstacleRects.erase(obstacleRects.begin());
    }

    // Generate new obstacle
    if (rand() % 300 < 5) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        obstacleRects.push_back({ width, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight });
    }

    for (auto& rect : powerUpRects) {
        rect.x -= 5;
    }

    if (!powerUpRects.empty() && powerUpRects[0].x + powerUpWidth < 0) {
        powerUpRects.erase(powerUpRects.begin());
        powerUpTypes.erase(powerUpTypes.begin());
    }

    // Generate new power-up
    if (rand() % 1000 < 5) {
        powerUpRects.push_back({ width, height - groundHeight - powerUpHeight, powerUpWidth, powerUpHeight });
        PowerUpType type = static_cast<PowerUpType>(rand() % 3 + 1);
        powerUpTypes.push_back(type);
    }

    for (auto& rect : enemyRects) {
        rect.x -= 3; // Move enemies slower than obstacles
    }

    // Remove off-screen enemies
    if (!enemyRects.empty() && enemyRects[0].x + obstacleWidth < 0) {
        enemyRects.erase(enemyRects.begin());
    }

    // Generate new enemy
    if (rand() % 500 < 5) {
        int enemyHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        enemyRects.push_back({ width, height - groundHeight - enemyHeight, obstacleWidth, enemyHeight });
    }

    if (isJumping) {
        playerRect.y -= jumpSpeed;
        if (playerRect.y <= playerPosY - jumpHeight) {
            isJumping = false;
            isFalling = true;
        }
    }

    if (isFalling) {
        playerRect.y += fallSpeed;
        for (const auto& rect : obstacleRects) {
            if (SDL_HasIntersection(&playerRect, &rect)) {
                isFalling = false;
                playerRect.y = rect.y - playerHeight;
                break;
            }
        }
        if (isFalling && playerRect.y >= playerPosY) {
            playerRect.y = playerPosY;
            isFalling = false;
        }
    }

    if (currentPowerUp != NONE) {
        powerUpDuration--;
        if (powerUpDuration <= 0) {
            currentPowerUp = NONE;
        }
    }
}

bool checkCollision() {
    for (const auto& rect : obstacleRects) {
        if (SDL_HasIntersection(&playerRect, &rect) && currentPowerUp != INVINCIBLE) {
            return true;
        }
    }
    for (const auto& rect : enemyRects) {
        if (SDL_HasIntersection(&playerRect, &rect) && currentPowerUp != INVINCIBLE) {
            return true;
        }
    }
    return false;
}

bool checkPowerUpCollection() {
    for (size_t i = 0; i < powerUpRects.size(); i++) {
        if (SDL_HasIntersection(&playerRect, &powerUpRects[i])) {
            currentPowerUp = powerUpTypes[i];
            powerUpDuration = 300;
            powerUpRects.erase(powerUpRects.begin() + i);
            powerUpTypes.erase(powerUpTypes.begin() + i);
            return true;
        }
    }
    return false;
}

int main(int argc, char* args[]) {
    srand(static_cast<unsigned int>(time(0)));
    initSDL();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                if (gameState == MENU) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            selectedMenuOption = (selectedMenuOption - 1 + 2) % 2;
                            break;
                        case SDLK_DOWN:
                            selectedMenuOption = (selectedMenuOption + 1) % 2;
                            break;
                        case SDLK_RETURN:
                            if (selectedMenuOption == 0) {
                                gameState = PLAYING;
                            } else {
                                quit = true;
                            }
                            break;
                    }
                } else if (gameState == PLAYING) {
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if (!isJumping && !isFalling) {
                                isJumping = true;
                            }
                            break;
                    }
                }
            }
        }

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
        }

        this_thread::sleep_for(chrono::milliseconds(16));
    }

    closeSDL();
    return 0;
}
