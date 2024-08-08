#include "Enemy.h"

Enemy::Enemy(SDL_Rect rect) : rect(rect) {}

SDL_Rect Enemy::getRect() {
    return rect;
}

void Enemy::update() {
    rect.x -= 2;
}

void Enemy::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}
