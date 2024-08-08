#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <vector>

class Player {
public:
    Player();
    SDL_Rect getRect();
    void handleEvent(SDL_Event& e);
    void update(const std::vector<SDL_Rect>& obstacleRects);
    void render(SDL_Renderer* renderer);

private:
    SDL_Rect playerRect;
    bool isJumping;
    bool isFalling;
    int jumpSpeed;
    int fallSpeed;
    int jumpHeight;
    int playerPosY;
    int moveSpeed;
};

#endif
