#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include "raylib.h"
#include "map"
#include "string"
#include "iostream"

class AnimatedSprite
{
private:
    int frameCount;
    Rectangle source;

public:
    int curFrame;
    bool flipped;
    bool complete;
    Vector2 spriteSize;
    Vector2 position;
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
        bool looping;
        bool finished;
        bool playing;
    };
    std::map<std::string, animation> Animations;
    std::string currentAnimation;
    void addAnimation(std::string name, int row, int startFrame, int frames, int fps, bool looping);
    void playAnimation(std::string name);
    void Update();
    AnimatedSprite();
    AnimatedSprite(const std::string SpriteSheet, Vector2 spriteSize);
};

#endif