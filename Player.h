#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Bullet.h"

class Player {
public:
    Player(int screenWidth, int screenHeight);
    //~Player() ;

    void handleInput(SDL_Keycode key, std::vector<Bullet>& bullets);
    void update();
    void draw(SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    void reset();
    void setJumpHeight(int height);
    void setJumpSpeed(int speed);
    void setFlying(bool flying);

    SDL_Rect getRect() const;
    SDL_Renderer* renderer;
    SDL_Texture* loadTexture(const std::string &path);

private:
    SDL_Rect playerRect;
    int currentFrame; // Khung hình hiện tại trong sprite sheet
    int frameWidth; // Chiều rộng của mỗi khung hình
    int frameHeight; // Chiều cao của mỗi khung hình
    int totalFrames; // Tổng số khung hình trong hoạt ảnh
    int frameSpeed; // Tốc độ chuyển khung hình
    std::vector<SDL_Texture*> runTextures;
    int frameDelay;
    Uint32 lastFrameTime;

    bool isJumping;
    bool isFalling;
    bool isFlying;
    int jumpSpeed;
    int fallSpeed;
    int flySpeed;
    int flyHeight;
    int currentJumpHeight;
    int playerSpeed;
    int width;
    int height;
};

#endif // PLAYER_H
