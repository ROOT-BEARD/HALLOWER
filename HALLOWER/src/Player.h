#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Timer.h"
#include "AnimatedSprite.h"
#include <string>

class Player
{
private:
    void addAnimations();
    AnimatedSprite playerRender;
    Vector2 dir;
    float zPos;
    float acc;
    float topSpeed;
    float burrowTime;
    float burrowSpeed;
    float walkSpeed;
    float cooldown;
    Timer burrowTimer;
    Timer burrowCooldown;
    float hangTime;
    Timer hangTimer;
    bool grounded;
    float jumpVel;
    enum PLAYERSTATE
    {
        IDLE,
        WALKING,
        BURROWING,
        JUMPING,
        DIVING
    };
    enum ANIMATIONSTATE
    {
        idle,
        walking,
        burrowing,
        jumping,
        jumpAnticipation,
        falling,
        diving
    };
    enum RENDERDIR
    {
        UP,
        DOWN,
        HORIZONTAL
    };

    ANIMATIONSTATE animationState;
    RENDERDIR renderDir;
    PLAYERSTATE playerState;

    Vector2 Normalize(const Vector2 &oldDir) const;

    std::string animationChart[7][3] = {{"idle(up)", "idle(down)", "idle(horizontal)"},
                                        {"walk(up)", "walk(down)", "walk(horizontal)"},
                                        {"burrow", "burrow", "burrow"},
                                        {"jump(up)", "jump(down)", "jump(horizontal)"},
                                        {"jumpAnticipation(up)", "jumpAnticipation(down)", "jumpAnticipation(horizontal)"},
                                        {"falling(up)", "falling(down)", "falling(horizontal)"},
                                        {
                                            "dive",
                                            "dive",
                                            "dive",
                                        }};

public:
    float curSpeed;
    Rectangle render;
    Vector2 playerPos;
    Player();
    void getDir();
    void Move(float speed);
    void Draw();
    void Update();
    void Colliding();
};

#endif