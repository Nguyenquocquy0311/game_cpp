#include "Player.h"
#include "Game.h"
#include <SDL_mixer.h>

Player::Player(int screenWidth, int screenHeight)
    : isJumping(false), isFalling(false), isFlying(false), jumpSpeed(8), fallSpeed(8), flySpeed(5),
      currentJumpHeight(20), playerSpeed(10), width(1200) {
    playerRect = { 300, screenHeight - 250, 20, 40};
    width = screenWidth;
    height = screenHeight;
}

void Player::handleInput(SDL_Keycode key, std::vector<Bullet>& bullets) {
    if (key == SDLK_UP && isFlying) {
        playerRect.y -= flySpeed;
    } else if (key == SDLK_UP && !isJumping && !isFalling && !isFlying) {
        isJumping = true;
        Mix_PlayChannel(-1, gJumpSound, 0);
    } else if (key == SDLK_SPACE) {
        Bullet newBullet;
        newBullet.bulletRect = { playerRect.x + playerRect.w, playerRect.y + playerRect.h / 2 - 5, 10, 5 };
        newBullet.speed = 10;
        bullets.push_back(newBullet);
        Mix_PlayChannel(-1, gShootSound, 0);
    }
}

void Player::update() {
    if (isFlying) {
        if (playerRect.y > 0) {
            playerRect.y -= flySpeed;
        }

        if (playerRect.y < height - 240 - playerRect.h) {
            playerRect.y += flySpeed;
        }
    } else {
        if (isJumping) {
            playerRect.y -= jumpSpeed;
            if (playerRect.y <= height - 240 - currentJumpHeight - playerRect.h) {
                isJumping = false;
                isFalling = true;
            }
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
    if (state[SDL_SCANCODE_LEFT] && !isJumping && !isFlying) {
        playerRect.x -= playerSpeed;
        if (playerRect.x < 0) {
            playerRect.x = 0;
        }
    }
    if (state[SDL_SCANCODE_RIGHT] && !isJumping && !isFlying) {
        playerRect.x += playerSpeed;
        if (playerRect.x > width - playerRect.w) {
            playerRect.x = width - playerRect.w;
        }
    }
}

void Player::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, NULL, &playerRect);
}


void Player::reset() {
    playerRect = { 100, height - 240, 100, 100 };
    isJumping = false;
    isFalling = false;
    isFlying = false;
    currentJumpHeight = 120;
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
}

