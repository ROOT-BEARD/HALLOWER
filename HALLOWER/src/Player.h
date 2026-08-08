#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Timer.h"
#include "AnimatedSprite.h"
#include <string>
#include "vector"
#include "Tile.h"
#include "WorldManager.h"

class Player
{
private:
    void getDir();
    void Move(float speed);
    void Jump();
    void Attack();
    bool ShouldCollide(const Tile &tile);
    Vector2 Normalize(const Vector2 &oldDir) const;

    Rectangle attackArea;
    bool attackActive;
    struct playerStats
    {
        float acc = 300.0f;
        float burrowTime = 1.0f;
        float burrowSpeed = 100.0f;
        float walkSpeed = 50.0f;
        float cooldown = 1.25f;
        float jumpVel = 200.0f;
        float gravity = 150.0f;
        float hangTime = 0.35f;
        float jumpHeight = 12.0f;
    };
    playerStats stats;
    bool burrowJump = false;
    float curSpeed;
    void addAnimations();
    AnimatedSprite playerRender;
    Vector2 dir;
    float zPos;
    Timer burrowTimer;
    Timer burrowCooldown;
    float hangTime;
    Timer hangTimer;
    Timer groundedTimer;
    Timer jumpBuffer;
    float bufferAmount;
    bool grounded;
    enum PLAYERSTATE
    {
        IDLE,
        WALKING,
        BURROWING,
        JUMPING,
        ATTACKING
    };
    enum ANIMATIONSTATE
    {
        idle = 0,
        walking,
        burrowing,
        jumping,
        falling,
        attacking
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

    std::string animationChart[6][3] = {{"idle(up)", "idle(down)", "idle(horizontal)"},
                                        {"walk(up)", "walk(down)", "walk(horizontal)"},
                                        {"burrow", "burrow", "burrow"},
                                        {"jump(up)", "jump(down)", "jump(horizontal)"},
                                        {"falling(up)", "falling(down)", "falling(horizontal)"},
                                        {"attack(up)", "attack(down)", "attack(horizontal)"}};

public:
    Rectangle collision;
    Vector2 playerPos;
    std::vector<Tile *> nearbyTiles;
    Player();
    void Draw();
    void Update();
    void Colliding();
};

#endif