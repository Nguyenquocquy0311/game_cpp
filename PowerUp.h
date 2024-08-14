#ifndef POWERUP_H
#define POWERUP_H

#include <SDL.h>
#include <string>

enum PowerUpType {
    HIGH_JUMP,
    FLY,
    INVINCIBLE,
    NONE
};

class PowerUp {
public:
    PowerUp(int x, int y, int width, int height, PowerUpType type);
    ~PowerUp();
    void update();
    void draw(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;
    PowerUpType getType() const;
    void loadTextures(SDL_Renderer* renderer, const std::string& highJumpPath, const std::string& flyPath, const std::string& invinciblePath);
    void setTexture(SDL_Texture* highJumpTexture, SDL_Texture* flyTexture, SDL_Texture* invincibleTexture);

private:
    SDL_Rect powerUpRect;
    PowerUpType type;
    SDL_Texture* texture;
};

#endif // POWERUP_H
