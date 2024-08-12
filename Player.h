#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "Bullet.h"

class Player {
public:
    Player(int screenWidth, int screenHeight);
    void handleInput(SDL_Keycode key, std::vector<Bullet>& bullets);
    void update();
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    void reset();
    SDL_Rect getRect() const;
    void setJumpHeight(int height);
    void setJumpSpeed(int speed);
    void setFlying(bool flying);

private:
    SDL_Rect playerRect;
    bool isJumping;
    bool isFalling;
    bool isFlying;
    int jumpSpeed;
    int fallSpeed;
    int flySpeed;
    int currentJumpHeight;
    int playerSpeed;
    int width;
    int height;
};

#endif // PLAYER_H
