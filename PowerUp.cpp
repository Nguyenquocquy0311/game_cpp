#include "PowerUp.h"
#include <SDL_image.h>
#include <iostream>

PowerUp::PowerUp(int x, int y, int width, int height, PowerUpType type)
    : type(type), texture(nullptr), alpha(255), alphaDirection(-5) {
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

    alpha += alphaDirection;
    if (alpha <= 100 || alpha >= 255) {
        alphaDirection = -alphaDirection; // Đảo hướng khi alpha đạt ngưỡng
    }
}

void PowerUp::draw(SDL_Renderer* renderer) const {
    if (texture != nullptr) {
        //SDL_RenderCopy(renderer, texture, nullptr, &powerUpRect);

        SDL_SetTextureAlphaMod(texture, alpha);
        SDL_RenderCopy(renderer, texture, nullptr, &powerUpRect);
    }
}

SDL_Rect PowerUp::getRect() const {
    return powerUpRect;
}

PowerUpType PowerUp::getType() const {
    return type;
}
