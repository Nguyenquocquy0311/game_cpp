#include "Bullet.h"

Bullet::Bullet() : speed(50) {
    bulletRect = { 0, 0, 50, 50 };
}

void Bullet::update() {
    bulletRect.x += speed;
}

void Bullet::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &bulletRect);
}

SDL_Rect Bullet::getRect() const {
    return bulletRect;
}
