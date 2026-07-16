#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Timer.h"

class Player
{
private:
    Vector2 dir;
    float zPos;
    float acc;
    float topSpeed;
    bool burrowed;
    float burrowTime;
    float burrowSpeed;
    float walkSpeed;
    float cooldown;
    bool walking;
    Timer burrowTimer;
    Timer burrowCooldown;
    float hangTime;
    Timer hangTimer;
    bool grounded;
    float jumpVel;
    bool jumping;
    typedef enum
    {
        IDLE,
        WALKING,
        BURROWING,
        JUMPING,
        DIVING
    } PLAYERSTATE;
    PLAYERSTATE playerState;
    Texture2D playerRender;
    Vector2 renderDir;

    Vector2 Normalize(Vector2 oldDir);

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