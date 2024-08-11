#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    Bullet();
    void update();
    void draw(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;

    SDL_Rect rect;
    int speed;
};


#endif // BULLET_H
