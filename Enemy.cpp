#include "Enemy.h"
#include <iostream>

Enemy::Enemy(int x, int y, int width, int height, int direction)
    : direction(direction), speed(1) {
    enemyRect = { x, y, width, height };
}

void Enemy::update() {
    switch (direction) {
        case 0: // Di chuyển từ phải sang trái
            enemyRect.x -= speed;
            break;
        case 1: // Di chuyển từ trái sang phải
            enemyRect.x += speed;
            break;
        case 2: // Di chuyển từ trên xuống dưới
            enemyRect.y += speed;
            break;
        case 3: // Di chuyển từ dưới lên trên
            enemyRect.y -= speed;
            break;
    }
}

void Enemy::setTexture(SDL_Texture* texture) {
    enemyTexture = texture;
}

void Enemy::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, NULL, &enemyRect);

    std::cout << "Drawing enemy at x: " << enemyRect.x << ", y: " << enemyRect.y << std::endl; // Debugging statement
}

void Enemy::draw(SDL_Renderer* renderer) const {
    SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemyRect);
}

SDL_Rect Enemy::getRect() const {
    return enemyRect;
}
