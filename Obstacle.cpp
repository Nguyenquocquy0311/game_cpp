#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height) {
    obstacleRect = { x, y, width, height };
}

void Obstacle::update() {
    obstacleRect.x -= 5;
}

void Obstacle::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &obstacleRect);
}

SDL_Rect Obstacle::getRect() const {
    return obstacleRect;
}
