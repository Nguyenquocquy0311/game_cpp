#include "Game.h"

const int width = 1200;
const int height = 600;
const int playerWidth = 50;
const int playerHeight = 50;
const int playerPosY = height - playerHeight - 100;
const int playerPosX = 50;
const int jumpHeight = 120;
const int obstacleWidth = 20;
const int powerUpWidth = 40;
const int powerUpHeight = 40;
const int groundHeight = 100;
const int minObstacleHeight = 20;
const int maxObstacleHeight = 100;

Game::Game()
    : window(nullptr), renderer(nullptr), playerTexture(nullptr), invincibleTexture(nullptr),
      highJumpTexture(nullptr), longJumpTexture(nullptr), backgroundTexture(nullptr),
      isJumping(false), isFalling(false), gameOver(false), jumpSpeed(8), fallSpeed(8),
      currentPowerUp(NONE), powerUpDuration(0), gameState(MENU), selectedMenuOption(0) {
    playerRect = { playerPosX, playerPosY, playerWidth, playerHeight };
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
    IMG_Init(IMG_INIT_PNG);
    loadTextures();
}

void Game::closeSDL() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(invincibleTexture);
    SDL_DestroyTexture(highJumpTexture);
    SDL_DestroyTexture(longJumpTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Game::loadTextures() {
    playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    invincibleTexture = IMG_LoadTexture(renderer, "assets/invincible.png");
    highJumpTexture = IMG_LoadTexture(renderer, "assets/high_jump.png");
    longJumpTexture = IMG_LoadTexture(renderer, "assets/long_jump.png");
    backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");

    if (!playerTexture || !invincibleTexture || !highJumpTexture || !longJumpTexture || !backgroundTexture) {
        std::cerr << "Failed to load textures! SDL Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void Game::drawMenu() {
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

void Game::draw() {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);

    SDL_Rect groundRect = { 0, height - groundHeight, width, groundHeight };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &groundRect);

    SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

    for (const auto& rect : obstacleRects) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }

    for (size_t i = 0; i < powerUpRects.size(); i++) {
        SDL_Texture* powerUpTexture = nullptr;
        switch (powerUpTypes[i]) {
            case HIGH_JUMP:
                powerUpTexture = highJumpTexture;
                break;
            case LONG_JUMP:
                powerUpTexture = longJumpTexture;
                break;
            case INVINCIBLE:
                powerUpTexture = invincibleTexture;
                break;
            default:
                break;
        }
        SDL_RenderCopy(renderer, powerUpTexture, NULL, &powerUpRects[i]);
    }

    for (const auto& rect : enemyRects) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
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

void Game::update() {
    if (gameOver) {
        return;
    }

    for (auto& rect : obstacleRects) {
        rect.x -= 5;
    }

    if (!obstacleRects.empty() && obstacleRects[0].x + obstacleWidth < 0) {
        obstacleRects.erase(obstacleRects.begin());
    }

    for (auto& rect : powerUpRects) {
        rect.x -= 5;
    }

    if (!powerUpRects.empty() && powerUpRects[0].x + powerUpWidth < 0) {
        powerUpRects.erase(powerUpRects.begin());
        powerUpTypes.erase(powerUpTypes.begin());
    }

    for (auto& rect : enemyRects) {
        rect.x -= 3;
    }

    if (!enemyRects.empty() && enemyRects[0].x + obstacleWidth < 0) {
        enemyRects.erase(enemyRects.begin());
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

void Game::generateObstacles() {
    obstacleRects.clear();
    for (int i = 0; i < 10; ++i) {
        int obstacleHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        obstacleRects.push_back({ width + i * 200, height - groundHeight - obstacleHeight, obstacleWidth, obstacleHeight });
    }
}

void Game::generateEnemies() {
    enemyRects.clear();
    for (int i = 0; i < 5; ++i) {
        int enemyHeight = minObstacleHeight + rand() % (maxObstacleHeight - minObstacleHeight + 1);
        enemyRects.push_back({ width + i * 400, height - groundHeight - enemyHeight, obstacleWidth, enemyHeight });
    }
}

void Game::generatePowerUps() {
    powerUpRects.clear();
    powerUpTypes.clear();
    for (int i = 0; i < 3; ++i) {
        powerUpRects.push_back({ width + i * 600, height - groundHeight - powerUpHeight, powerUpWidth, powerUpHeight });
        powerUpTypes.push_back(static_cast<PowerUpType>(rand() % 3 + 1));
    }
}

bool Game::checkCollision() {
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

bool Game::checkPowerUpCollection() {
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

void Game::run() {
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
                                generateObstacles();
                                generateEnemies();
                                generatePowerUps();
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

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    closeSDL();
}