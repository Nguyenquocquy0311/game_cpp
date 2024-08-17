#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "Bullet.h"
#include "Item.h"

class Player {
public:
    Player(int screenWidth, int screenHeight);
    //~Player() ;

    void handleInput(SDL_Keycode key, std::vector<Bullet>& bullets);
    void update();
    void draw(SDL_Renderer* renderer);
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    void draw(SDL_Renderer* renderer, SDL_Texture* texture, TTF_Font* font);
    void drawStaminaBar(SDL_Renderer* renderer, TTF_Font* font) const;
    void reset();
    void setJumpHeight(int height);
    void setJumpSpeed(int speed);
    void setFlying(bool flying);
    void collectItem(ItemType type);
    void collectAxe(bool hasAxe);
    void increaseStamina(int amount);
    bool canBreakObstacle() const;
    int getBulletCount() const;
    void increaseBullets(int bullet);
    void resetBullet();

    SDL_Rect getRect() const;
    SDL_Renderer* renderer;
    SDL_Texture* loadTexture(const std::string &path);

private:
    SDL_Rect playerRect;
    int currentFrame;
    int frameWidth;
    int frameHeight;
    int totalFrames;
    int frameSpeed;
    std::vector<SDL_Texture*> runTextures;
    int frameDelay;
    Uint32 lastFrameTime;

    bool isJumping;
    bool isFalling;
    bool isFlying;
    bool hasAxe;
    int jumpSpeed;
    int fallSpeed;
    int flySpeed;
    int flyHeight;
    int currentJumpHeight;
    int playerSpeed;
    int stamina;
    int bulletCount;
    int width;
    int height;
};

#endif // PLAYER_H
