#include "Player.h"
#include "Game.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

Player::Player(int screenWidth, int screenHeight)
    : isJumping(false), isFalling(false), isFlying(false), hasAxe(false), jumpSpeed(8), fallSpeed(8), flySpeed(15),
      currentJumpHeight(80), playerSpeed(10), currentFrame(0), frameWidth(107), frameHeight(100),
      totalFrames(6), frameSpeed(4), flyHeight(500), stamina(250), bulletCount(20) {
    playerRect = { 300, screenHeight - 250, 100, 100};
    width = screenWidth;
    height = screenHeight;
}

void Player::handleInput(SDL_Keycode key, std::vector<Bullet>& bullets) {
    if (isFlying) {
        if (key == SDLK_UP) {
            flyHeight = playerRect.y - flySpeed;
            if (playerRect.y < 0) {
                playerRect.y = 0;
            }
        } else if (key == SDLK_DOWN) {
            flyHeight = playerRect.y + flySpeed;
            if (playerRect.y + playerRect.h > height) {
                playerRect.y = height - playerRect.h - 210;
            }
        } else if (key == SDLK_SPACE && bulletCount > 0) {
            Bullet newBullet;
            newBullet.bulletRect = { playerRect.x - 10 + playerRect.w, playerRect.y + playerRect.h / 2 + 10, 30, 10 };
            newBullet.speed = 15;
            bullets.push_back(newBullet);
            Mix_PlayChannel(-1, gShootSound, 0);
            bulletCount--;
        }
    } else {
        if (key == SDLK_UP && !isJumping && !isFalling && stamina > 0) {
            isJumping = true;
            Mix_PlayChannel(-1, gJumpSound, 0);
            currentFrame = 0;
            stamina -= 10;
        } else if (key == SDLK_SPACE && bulletCount > 0) {
            Bullet newBullet;
            newBullet.bulletRect = { playerRect.x - 10 + playerRect.w, playerRect.y + playerRect.h / 2 + 10, 30, 10 };
            newBullet.speed = 15;
            bullets.push_back(newBullet);
            Mix_PlayChannel(-1, gShootSound, 0);
            bulletCount--;
        }
    }
}

void Player::update() {
    if (isFlying) {
        currentFrame = 0;
        int tolerance = 5;

        if (abs(playerRect.y - flyHeight) > tolerance) {
            if (playerRect.y > flyHeight) {
                playerRect.y -= flySpeed;
            } else if (playerRect.y < flyHeight) {
                playerRect.y += flySpeed;
            }
        } else {
            playerRect.y = flyHeight;
        }

        if (playerRect.y + playerRect.h > height - 180) {
            playerRect.y = height - 180 - playerRect.h;
        }

    } else {
        if (isJumping) {
            playerRect.y -= jumpSpeed;
            if (playerRect.y <= height - 250 - currentJumpHeight - playerRect.h) {
                isJumping = false;
                isFalling = true;
            }
            currentFrame = 0;
        }

        if (isFalling) {
            playerRect.y += fallSpeed;
            if (playerRect.y >= height - 250) {
                playerRect.y = height - 250;
                isFalling = false;
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT]) {
        playerRect.x -= playerSpeed;
        if (playerRect.x < 0) {
            playerRect.x = 0;
            stamina -= 10;
        }
    } else if (state[SDL_SCANCODE_RIGHT]) {
        playerRect.x += playerSpeed;
        if (playerRect.x > width - playerRect.w) {
            playerRect.x = width - playerRect.w;
            stamina -= 5;
        }

        currentFrame++;
        if (currentFrame >= totalFrames * frameSpeed) {
            currentFrame = 0;
        }
    } else if (!isJumping && !isFlying) {
        currentFrame++;
        if (currentFrame >= totalFrames * frameSpeed) {
            currentFrame = 0;
        }
    }
}


void Player::draw(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font) {
    SDL_Rect srcRect = { (currentFrame / frameSpeed) * frameWidth, 0, frameWidth, frameHeight };
    SDL_RenderCopy(renderer, texture, &srcRect, &playerRect);

    drawStaminaBar(renderer, font);
}

void Player::drawStaminaBar(SDL_Renderer* renderer, TTF_Font* font) const {
    int barWidth = 100;
    int barHeight = 10;

    int currentBarWidth = (stamina * barWidth) / 200;

    int barX = playerRect.x;
    int barY = playerRect.y - barHeight - 5;

    SDL_Rect staminaBarBackground = { barX, barY, barWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &staminaBarBackground);

    SDL_Rect staminaBarForeground = { barX, barY, currentBarWidth, barHeight };
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
    SDL_RenderFillRect(renderer, &staminaBarForeground);

    std::string staminaText = std::to_string(stamina);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, staminaText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    int textX = barX + barWidth + 5;
    int textY = barY - 7;

    int textW = textSurface->w;
    int textH = textSurface->h;

    SDL_Rect textRect = { textX, textY, textW, textH };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


void Player::reset() {
    playerRect = { 100, height - 250, 100, 100 };
    isJumping = false;
    isFalling = false;
    isFlying = false;
    currentJumpHeight = 80;
    flyHeight = 500;
    stamina = 200;
}

SDL_Rect Player::getRect() const {
    return playerRect;
}

void Player::setJumpHeight(int height) {
    currentJumpHeight = height;
}

void Player::setJumpSpeed(int speed) {
    playerSpeed = speed;
}

void Player::setFlying(bool flying) {
    isFlying = flying;
    if (flying) {
        flyHeight = playerRect.y - 200;
        currentFrame = 0;
    } else {
        if (playerRect.y < height - 250) {
            isFalling = true;
        }
    }
}

void Player::collectAxe(bool hasAxe) {
    this->hasAxe = hasAxe;
}

void Player::increaseStamina(int amount) {
    stamina += amount;

    if (stamina > 200) {
        stamina = 200;
    }
}

bool Player::canBreakObstacle() const {
    return hasAxe;
}

void Player::increaseBullets(int bullet) {
    bulletCount += bullet;
}

int Player::getBulletCount() const {
    return bulletCount;
}

void Player::resetBullet() {
    bulletCount = 20;
}


