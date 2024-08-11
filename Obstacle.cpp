#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height) {
    obstacleRect = { x, y, width, height };
}

void Obstacle::update() {
    obstacleRect.x -= 5;
}

void Obstacle::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &obstacleRect);
}

SDL_Rect Obstacle::getRect() const {
    return obstacleRect;
}
