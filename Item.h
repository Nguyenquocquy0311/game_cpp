#ifndef ITEM_H
#define ITEM_H

#include <SDL.h>

enum ItemType {
    AXE,
    MAP,
    DRUMSTICK,
    BULLET
};

class Item {
public:
    Item(int x, int y, int w, int h, ItemType type);
    void draw(SDL_Renderer* renderer, SDL_Texture* texture);
    SDL_Rect getRect() const;
    ItemType getType() const;

private:
    SDL_Rect itemRect;
    ItemType type;
};

#endif // ITEM_H
