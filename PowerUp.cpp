#include "PowerUp.h"
#include <SDL_image.h>
#include <iostream>

PowerUp::PowerUp(int x, int y, int width, int height, PowerUpType type)
    : type(type), texture(nullptr) {
    powerUpRect = { x, y, width, height };
}

PowerUp::~PowerUp() {

}

void PowerUp::setTexture(SDL_Texture* highJumpTexture, SDL_Texture* flyTexture, SDL_Texture* invincibleTexture) {
    switch (type) {
        case HIGH_JUMP:
            texture = highJumpTexture;
            break;
        case FLY:
            texture = flyTexture;
            break;
        case INVINCIBLE:
            texture = invincibleTexture;
            break;
    }
}

void PowerUp::update() {
    powerUpRect.x -= 5;
}

void PowerUp::draw(SDL_Renderer* renderer) const {
    if (texture != nullptr) {
        SDL_RenderCopy(renderer, texture, nullptr, &powerUpRect);
    }
}

SDL_Rect PowerUp::getRect() const {
    return powerUpRect;
}

PowerUpType PowerUp::getType() const {
    return type;
}
