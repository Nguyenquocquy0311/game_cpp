#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int x, int y, int width, int height, int groundHeight, bool isRunning)
    : speed(8), currentFrame(0), frameSpeed(4), isRunning(isRunning), groundHeight(groundHeight) {
    enemyRect = { x, y, width, height };

    if (isRunning) {
        frameWidth = width;
        frameHeight = height;
        totalFrames = 4;
        speed = 10;
    } else {
        frameWidth = width;
        frameHeight = height;
        totalFrames = 4;
    }
}

void Enemy::setTexture(SDL_Texture* texture) {
    enemyTexture = texture;
}

void Enemy::update() {
    enemyRect.x -= speed;

    // Cập nhật frame cho animation
    currentFrame++;
    if (currentFrame >= totalFrames * frameSpeed) {
        currentFrame = 0;
    }
}

void Enemy::draw(SDL_Renderer* renderer) const {
    if (!enemyTexture) {
        std::cerr << "No texture set for enemy!" << std::endl;
        return;
    }

    SDL_Rect srcRect = { (currentFrame / frameSpeed) * frameWidth, 0, frameWidth, frameHeight };
    SDL_RenderCopy(renderer, enemyTexture, &srcRect, &enemyRect);
}

SDL_Rect Enemy::getRect() const {
    return enemyRect;
}
