#include "Item.h"

Item::Item(int x, int y, int w, int h, ItemType type)
    : itemRect({x, y, w, h}), type(type) {}

void Item::draw(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &itemRect);
}

SDL_Rect Item::getRect() const {
    return itemRect;
}

ItemType Item::getType() const {
    return type;
}
