#ifndef ENEMY_H
#define ENEMY_H

#include <SDL.h>

class Enemy {
public:
    Enemy(int x, int y, int width, int height, int direction);
    void setTexture(SDL_Texture* texture);
    void update();
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    void draw(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;

private:
    SDL_Rect enemyRect;
    SDL_Texture* enemyTexture;
    int direction; // 0: trái -> phải, 1: phải -> trái, 2: trên -> dưới, 3: dưới -> trên
    int speed;
};

#endif // ENEMY_H
