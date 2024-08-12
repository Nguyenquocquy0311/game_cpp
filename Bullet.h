#ifndef BULLET_H
#define BULLET_H

#include <SDL.h>
#include <SDL_image.h>

class Bullet {
public:
    Bullet();
    void update();
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    SDL_Rect getRect() const;

    SDL_Rect bulletRect;
    int speed;
};


#endif // BULLET_H
