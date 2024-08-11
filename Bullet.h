#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>

class Bullet {
public:
    SDL_Rect rect;
    int speed;

    Bullet(int x, int y, int width, int height, int speed);

    void update();
    bool isOffScreen(int screenWidth);
};

#endif
