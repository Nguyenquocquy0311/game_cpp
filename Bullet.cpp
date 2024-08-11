#include "Bullet.h"

Bullet::Bullet() : speed(20) {
    rect = { 0, 0, 10, 5 };
}

void Bullet::update() {
    rect.x += speed;
}

void Bullet::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Bullet::getRect() const {
    return rect;
}
