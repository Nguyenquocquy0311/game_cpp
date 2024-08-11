#ifndef POWERUP_H
#define POWERUP_H

#include <SDL.h>

enum PowerUpType { NONE, HIGH_JUMP, LONG_JUMP, INVINCIBLE };

class PowerUp {
public:
    PowerUp(int x, int y, int width, int height, PowerUpType type);
    void update();
    void draw(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;
    PowerUpType getType() const;

private:
    SDL_Rect powerUpRect;
    PowerUpType type;
};

#endif // POWERUP_H
