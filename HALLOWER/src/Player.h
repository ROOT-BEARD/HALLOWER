#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "raylib.h"
#include "Timer.h"
#include "AnimatedSprite.h"
#include "Tile.h"
#include "WorldManager.h"

class Player
{
private:
    // private functions
    void getDir();
    void Move(float speed, float delta);
    void Jump();
    void Attack();
    void addAnimations();
    bool ShouldCollide(const Tile &tile);
    Vector2 Normalize(const Vector2 &oldDir) const;

    // positioning
    Vector2 lastPos;
    Vector2 dir;
    float curSpeed;
    float zPos;

    // timers
    Timer hangTimer;
    Timer groundedTimer;
    Timer jumpBuffer;
    Timer burrowTimer;
    Timer burrowCooldown;

    // time amout for timers
    float hangTime;
    float bufferAmount;

    // attack
    Rectangle attackArea;
    bool attackActive;

    // the players stats
    struct playerStats
    {
        float acc = 200.0f;
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

    // player state logic
    enum PLAYERSTATE
    {
        IDLE,
        WALKING,
        BURROWING,
        JUMPING,
        ATTACKING,
        FALLINGPIT,
        SLIDING
    };
    PLAYERSTATE playerState;
    bool burrowJump = false;
    bool grounded;

    // rendering logic
    enum ANIMATIONSTATE
    {
        idle = 0,
        walking,
        burrowing,
        jumping,
        falling,
        attacking,
        fallingpit,
        sliding
    };
    enum RENDERDIR
    {
        UP,
        DOWN,
        HORIZONTAL
    };
    ANIMATIONSTATE animationState;
    RENDERDIR renderDir;
    AnimatedSprite playerRender;
    Texture2D shadowTexture;
    static constexpr const char *animationChart[8][3] = {{"idle(up)", "idle(down)", "idle(horizontal)"},
                                                         {"walk(up)", "walk(down)", "walk(horizontal)"},
                                                         {"burrow", "burrow", "burrow"},
                                                         {"jump(up)", "jump(down)", "jump(horizontal)"},
                                                         {"falling(up)", "falling(down)", "falling(horizontal)"},
                                                         {"attack(up)", "attack(down)", "attack(horizontal)"},
                                                         {"falling(pit)", "falling(pit)", "falling(pit)"},
                                                         {"sliding", "sliding", "sliding"}};

public:
    Texture2D spriteSheet = LoadTexture("Art/playerSheet.png");
    Rectangle collision;
    Vector2 playerPos;
    // the list of tiles within range
    std::vector<Tile *> nearbyTiles;
    // functions
    Player();
    ~Player();
    void Draw();
    void Update(float delta);
    void Collide();
    bool IsColliding();
};

#endif