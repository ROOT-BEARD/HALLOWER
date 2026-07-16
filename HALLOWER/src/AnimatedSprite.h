#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "raylib.h"
#include "map"
#include "string"
#include "iostream"

class AnimatedSprite
{
private:
    int curFrame;
    int frameCount;
    Rectangle source;

public:
    Vector2 spriteSize;
    Texture2D SpriteSheet;
    int SheetWidth;
    int SheetHeight;
    struct animation
    {
        std::string name;
        int row;
        int startFrame;
        int frames;
        int fps;
    };
    std::map<std::string, animation> Animations;
    std::string currentAnimation;
    void addAnimation(std::string name, int row, int startFrame, int frames, int fps);
    void playAnimation(std::string name);
    void Update();
    AnimatedSprite(const std::string SpriteSheet, Vector2 spriteSize);
};

#endif