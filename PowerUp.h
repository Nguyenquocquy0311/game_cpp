#ifndef POWERUP_H
#define POWERUP_H

enum PowerUpType { NONE, HIGH_JUMP, LONG_JUMP, INVINCIBLE };

class PowerUp {
public:
    PowerUp(SDL_Rect rect, PowerUpType type);
    SDL_Rect getRect();
    PowerUpType getType();

private:
    SDL_Rect rect;
    PowerUpType type;
};

#endif
