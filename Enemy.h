#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    Enemy(SDL_Rect rect);
    SDL_Rect getRect();
    void update();
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect rect;
};

#endif
