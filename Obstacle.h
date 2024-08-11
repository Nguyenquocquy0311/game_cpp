#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    SDL_Rect rect;

    Obstacle(int x, int y, int width, int height);

    void update();
    bool isOffScreen();
};

#endif
