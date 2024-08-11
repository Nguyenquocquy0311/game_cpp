#include "Bullet.h"

Bullet::Bullet(int x, int y, int width, int height, int speed)
    : speed(speed) {
    rect = { x, y, width, height };
}

void Bullet::update() {
    rect.x += speed;
}

bool Bullet::isOffScreen(int screenWidth) {
    return rect.x > screenWidth;
}
