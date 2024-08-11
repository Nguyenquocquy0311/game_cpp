#include "Enemy.h"

Enemy::Enemy(int x, int y, int width, int height, int direction)
    : direction(direction), speed(3) {
    enemyRect = { x, y, width, height };
}

void Enemy::update() {
    switch (direction) {
        case 0: // trái -> phải
            enemyRect.x += speed;
            break;
        case 1: // phải -> trái
            enemyRect.x -= speed;
            break;
        case 2: // trên -> dưới
            enemyRect.y += speed;
            break;
        case 3: // dưới -> trên
            enemyRect.y -= speed;
            break;
    }
}

void Enemy::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &enemyRect);
}

SDL_Rect Enemy::getRect() const {
    return enemyRect;
}
