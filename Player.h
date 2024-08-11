#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(int screenWidth, int screenHeight);
    void handleInput(SDL_Keycode key, std::vector<Bullet>& bullets);
    void update();
    void draw(SDL_Renderer* renderer);
    void reset();
    SDL_Rect getRect() const;
    void setJumpHeight(int height);
    void setJumpSpeed(int speed);

private:
    SDL_Rect playerRect;
    bool isJumping;
    bool isFalling;
    int jumpSpeed;
    int fallSpeed;
    int currentJumpHeight;
    int playerSpeed;
    int width;
    int height;
};

#endif // PLAYER_H
