#include "raylib.h"
#include "map"
#include "string"
#include "AnimatedSprite.h"
#include "iostream"
#include "string"

AnimatedSprite::AnimatedSprite(const std::string SpriteSheet, Vector2 spriteSize)
{
    this->SpriteSheet = LoadTexture(SpriteSheet.c_str());
    SheetHeight = this->SpriteSheet.height;
    SheetWidth = this->SpriteSheet.width;
    this->spriteSize = spriteSize;
    curFrame = 0;
    frameCount = 0;
    source = {0, (spriteSize.x * Animations[currentAnimation].row), spriteSize.x, spriteSize.y};
}

void AnimatedSprite::addAnimation(std::string name, int row, int startFrame, int frames, int fps)
{
    Animations[name] = animation{name, row, startFrame, frames, fps};
    std::cout << Animations.size() << std::endl;
}

void AnimatedSprite::playAnimation(std::string name)
{
    auto it = Animations.find(name);
    if (it != Animations.end())
    {
        currentAnimation = name;
        curFrame = Animations[currentAnimation].startFrame;
    }
}

void AnimatedSprite::Update()
{
    frameCount++;
    source.y = (spriteSize.y * Animations[currentAnimation].row);
    int curfps = Animations[currentAnimation].fps;
    if (frameCount >= (60 / curfps))
    {
        frameCount = 0;
        curFrame++;
        int endFrame = Animations[currentAnimation].startFrame + Animations[currentAnimation].frames;
        if (curFrame >= endFrame)
        {
            curFrame = Animations[currentAnimation].startFrame;
        }
        source.x = (curFrame * spriteSize.x);
    }
    DrawTextureRec(SpriteSheet, source, Vector2{60, 60}, WHITE);
}