#include "PowerUp.h"

PowerUp::PowerUp(SDL_Rect rect, PowerUpType type) : rect(rect), type(type) {}

SDL_Rect PowerUp::getRect() {
    return rect;
}

PowerUpType PowerUp::getType() {
    return type;
}
