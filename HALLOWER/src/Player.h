#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Timer.h"
#include "AnimatedSprite.h"

class Player
{
private:
    void addAnimations();
    AnimatedSprite playerRender = AnimatedSprite("Art/playerSheet.png", Vector2{24, 24});
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
    typedef enum
    {
        IDLE,
        WALKING,
        BURROWING,
        JUMPING,
        DIVING
    } PLAYERSTATE;
    typedef enum
    {
        idle,
        walking,
        burrowing,
        jumping,
        jumpAnticipation,
        falling,
        diving
    } AMIMATIONSTATE;
    typedef enum
    {
        UP,
        DOWN,
        HORIZONTAL
    } RENDERDIR;

    AMIMATIONSTATE animationState;
    RENDERDIR renderDir;
    PLAYERSTATE playerState;

    Vector2 Normalize(Vector2 oldDir);

    std::string animationChart[7][3] = {{"idle(up)", "idle(down)", "idle(horizontal)"},
                                        {"walk(up)", "walk(down)", "walk(horizontal)"},
                                        {"borrow", "borrow", "borrow"},
                                        {"jump(up)", "jump(down)", "jump(horizontal)"},
                                        {"jumpAnticipation(up)", "jumpAnticipation(down)", "jumpAnticipation(horizontal)"},
                                        {"falling(up)", "fallng(down)", "falling(horizaontal)"},
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