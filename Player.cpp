#include "Player.h"
#include "Game.h"
#include <SDL_mixer.h>

Player::Player(int screenWidth, int screenHeight)
    : isJumping(false), isFalling(false), isFlying(false), jumpSpeed(8), fallSpeed(8), flySpeed(15),
      currentJumpHeight(80), playerSpeed(10), currentFrame(0), frameWidth(107), frameHeight(100),
      totalFrames(6), frameSpeed(4), flyHeight(500) {
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
        } else if (key == SDLK_SPACE) {
            Bullet newBullet;
            newBullet.bulletRect = { playerRect.x - 10 + playerRect.w, playerRect.y + playerRect.h / 2 + 10, 30, 10 };
            newBullet.speed = 15;
            bullets.push_back(newBullet);
            Mix_PlayChannel(-1, gShootSound, 0);
        }
    } else {
        if (key == SDLK_UP && !isJumping && !isFalling) {
            isJumping = true;
            Mix_PlayChannel(-1, gJumpSound, 0);
            currentFrame = 0;
        } else if (key == SDLK_SPACE) {
            Bullet newBullet;
            newBullet.bulletRect = { playerRect.x - 10 + playerRect.w, playerRect.y + playerRect.h / 2 + 10, 30, 10 };
            newBullet.speed = 15;
            bullets.push_back(newBullet);
            Mix_PlayChannel(-1, gShootSound, 0);
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
        }
    } else if (state[SDL_SCANCODE_RIGHT]) {
        playerRect.x += playerSpeed;
        if (playerRect.x > width - playerRect.w) {
            playerRect.x = width - playerRect.w;
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


void Player::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_Rect srcRect = { (currentFrame / frameSpeed) * frameWidth, 0, frameWidth, frameHeight };
    SDL_RenderCopy(renderer, texture, &srcRect, &playerRect);
}

void Player::reset() {
    playerRect = { 100, height - 250, 100, 100 };
    isJumping = false;
    isFalling = false;
    isFlying = false;
    currentJumpHeight = 80;
    flyHeight = 500;
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

