#include "Player.h"

Player::Player(int screenWidth, int screenHeight) : isJumping(false), isFalling(false), jumpSpeed(8), fallSpeed(8), currentJumpHeight(120), playerSpeed(10), width(1200) {
    playerRect = { 300, 600 - 40 - 200, 20, 40 };
    width = screenWidth;
    height = screenHeight;
}

void Player::handleInput(SDL_Keycode key, std::vector<Bullet>& bullets) {
    if (key == SDLK_UP && !isJumping && !isFalling) {
        isJumping = true;
    } else if (key == SDLK_SPACE) {
        Bullet newBullet;
        newBullet.rect = { playerRect.x + playerRect.w, playerRect.y + playerRect.h / 2 - 5, 10, 5 };
        newBullet.speed = 10;
        bullets.push_back(newBullet);
    }
}

void Player::update() {
    if (isJumping) {
        playerRect.y -= jumpSpeed;
        if (playerRect.y <= 600 - 40 - 200 - currentJumpHeight) {
            isJumping = false;
            isFalling = true;
        }
    }

    if (isFalling) {
        playerRect.y += fallSpeed;
        if (playerRect.y >= 600 - 40 - 200) {
            playerRect.y = 600 - 40 - 200;
            isFalling = false;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_LEFT] && !isJumping) {
        playerRect.x -= playerSpeed;
        if (playerRect.x < 0) {
            playerRect.x = 0; // Giới hạn di chuyển trái
        }
    }
    if (state[SDL_SCANCODE_RIGHT] && !isJumping) {
        playerRect.x += playerSpeed;
        if (playerRect.x > width - playerRect.w) {
            playerRect.x = width - playerRect.w; // Giới hạn di chuyển phải
        }
    }
}

void Player::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerRect);
}

void Player::reset() {
    playerRect = { 300, 600 - 40 - 200, 20, 40 };
    isJumping = false;
    isFalling = false;
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
