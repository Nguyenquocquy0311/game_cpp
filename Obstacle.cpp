#include "Obstacle.h"

Obstacle::Obstacle(int x, int y, int width, int height) {
    rect = { x, y, width, height };
}

void Obstacle::update() {
    rect.x -= 5;
}

bool Obstacle::isOffScreen() {
    return rect.x + rect.w < 0;
}
