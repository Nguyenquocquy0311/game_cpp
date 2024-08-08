#include "Player.h"

const int playerWidth = 20;
const int playerHeight = 40;
const int playerPosX = 50;
const int jumpHeight = 120;
const int playerStartY = 400;

Player::Player() : playerRect{ playerPosX, playerStartY, playerWidth, playerHeight }, isJumping(false), isFalling(false), jumpSpeed(8), fallSpeed(8), jumpHeight(120), playerPosY(playerStartY), moveSpeed(5) {}

SDL_Rect Player::getRect() {
    return playerRect;
}

void Player::handleEvent(SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            case SDLK_UP:
                if (!isJumping && !isFalling) {
                    isJumping = true;
                }
                break;
            case SDLK_RIGHT:
                playerRect.x += moveSpeed;
                break;
        }
    }
}


void Player::update(const std::vector<SDL_Rect>& obstacleRects) {
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
}

void Player::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(renderer, &playerRect);
}
