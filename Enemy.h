#pragma once
#include <SDL.h>

class Enemy {
public:
    Enemy(int x, int y, int width, int height, int groundHeight, bool isRunning);

    void setTexture(SDL_Texture* texture);

    void update();
    void draw(SDL_Renderer* renderer) const;
    SDL_Rect getRect() const;

private:
    SDL_Rect enemyRect;
    SDL_Texture* enemyTexture;
    int speed;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int frameSpeed;
    bool isRunning;
    int groundHeight;
};
