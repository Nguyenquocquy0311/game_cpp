#include "PowerUp.h"
#include <SDL_image.h>
#include <iostream>

PowerUp::PowerUp(int x, int y, int width, int height, PowerUpType type)
    : type(type), highJumpTexture(nullptr), flyTexture(nullptr), invincibleTexture(nullptr) {
    powerUpRect = { x, y, width, height };
}

PowerUp::~PowerUp() {
    if (highJumpTexture) {
        SDL_DestroyTexture(highJumpTexture);
        highJumpTexture = nullptr;
    }
    if (flyTexture) {
        SDL_DestroyTexture(flyTexture);
        flyTexture = nullptr;
    }
    if (invincibleTexture) {
        SDL_DestroyTexture(invincibleTexture);
        invincibleTexture = nullptr;
    }
}

void PowerUp::loadTextures(SDL_Renderer* renderer, const std::string& highJumpPath, const std::string& flyPath, const std::string& invinciblePath) {
    SDL_Surface* loadedSurface = IMG_Load(highJumpPath.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << highJumpPath << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        highJumpTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }

    loadedSurface = IMG_Load(flyPath.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << flyPath << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        flyTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }

    loadedSurface = IMG_Load(invinciblePath.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << invinciblePath << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        invincibleTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
}

void PowerUp::update() {
    powerUpRect.x -= 5;
}

void PowerUp::draw(SDL_Renderer* renderer) const {
    SDL_Texture* texture = nullptr;

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
