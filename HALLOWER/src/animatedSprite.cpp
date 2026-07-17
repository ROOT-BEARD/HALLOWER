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
    flipped = false;
}

void AnimatedSprite::addAnimation(std::string name, int row, int startFrame, int frames, int fps, bool looping)
{
    Animations[name] = animation{name, row, startFrame, frames, fps, looping, false, false};
    std::cout << Animations.size() << std::endl;
}

void AnimatedSprite::playAnimation(std::string name)
{
    auto it = Animations.find(name);
    if (it != Animations.end())
    {
        if (currentAnimation != name)
        {
            complete = false;
            currentAnimation = name;
            curFrame = Animations[currentAnimation].startFrame;
            Animations[currentAnimation].finished = false;
            Animations[currentAnimation].playing = true;
            frameCount = 0;
        }
    }
}

void AnimatedSprite::Update()
{
    frameCount++;
    int curfps = Animations[currentAnimation].fps;
    if (Animations[currentAnimation].looping || !Animations[currentAnimation].finished)
    {
        if (frameCount >= (60 / curfps))
        {
            frameCount = 0;
            curFrame++;

            int endFrame = Animations[currentAnimation].startFrame + Animations[currentAnimation].frames;
            if (curFrame >= endFrame)
            {
                if (Animations[currentAnimation].looping)
                {
                    curFrame = Animations[currentAnimation].startFrame;
                }
                else
                {
                    curFrame = endFrame - 1;
                    Animations[currentAnimation].finished = true;
                }
                curFrame = Animations[currentAnimation].startFrame;
                complete = true;
            }
        }
    }
    source.y = (spriteSize.y * Animations[currentAnimation].row);
    source.x = (curFrame * spriteSize.x);
    source.width = spriteSize.x;
    if (flipped)
    {
        source.width = -source.width;
    }
    DrawTextureRec(SpriteSheet, source, position, WHITE);
}