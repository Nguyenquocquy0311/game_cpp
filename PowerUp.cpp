#include "PowerUp.h"

PowerUp::PowerUp(int x, int y, int width, int height, PowerUpType type)
    : type(type) {
    powerUpRect = { x, y, width, height };
}

void PowerUp::update() {
    powerUpRect.x -= 5;
}

void PowerUp::draw(SDL_Renderer* renderer) const {
    switch (type) {
        case HIGH_JUMP:
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Màu vàng cho HIGH_JUMP
            break;
        case LONG_JUMP:
            SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);  // Màu xanh dương nhạt cho LONG_JUMP
            break;
        case INVINCIBLE:
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);  // Màu hồng cho INVINCIBLE
            break;
        default:
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Màu trắng cho mặc định
            break;
    }
    SDL_RenderFillRect(renderer, &powerUpRect);
}

SDL_Rect PowerUp::getRect() const {
    return powerUpRect;
}

PowerUpType PowerUp::getType() const {
    return type;
}
