#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    Obstacle(int x, int y, int width, int height);
    void update();
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    SDL_Rect getRect() const;

private:
    SDL_Rect obstacleRect;
};

#endif // OBSTACLE_H
