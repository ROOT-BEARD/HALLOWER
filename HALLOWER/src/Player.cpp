#include "Player.h"
#include "Timer.h"
#include "iostream"
#include "cmath"
#include "AnimatedSprite.h"

Player::Player()
{
    dir = Vector2{0.0f, 0.0f};
    zPos = 0;
    playerPos = Vector2{32.0f, 32.0f};
    burrowTime = 2.0f; // two seconds
    cooldown = 1.25f;
    curSpeed = 0.0f;
    acc = 0.1f;
    walkSpeed = 1.5f;
    burrowSpeed = 3.0f;
    topSpeed = 3.0f;
    render = {playerPos.x, playerPos.y, 32, 32}; // used to hold what character the player is
    burrowTimer = Timer(burrowTime);
    burrowCooldown = Timer(cooldown);
    hangTime = 0.5f;
    hangTimer = Timer(hangTime);
    grounded = true;
    jumpVel = 250.0f;
    playerState = IDLE;
    renderDir = DOWN;
    addAnimations();
}

void Player::addAnimations()
{
    playerRender.addAnimation("spin", 0, 0, 4, 4, true);
    playerRender.addAnimation("walk(up)", 1, 8, 4, 4, true);
    playerRender.addAnimation("walk(down)", 1, 0, 4, 4, true);
    playerRender.addAnimation("walk(horizontal)", 1, 4, 4, 4, true);
    playerRender.addAnimation("idle(up)", 0, 2, 1, 1, true);
    playerRender.addAnimation("idle(down)", 0, 0, 1, 1, true);
    playerRender.addAnimation("idle(horizontal)", 0, 1, 1, 1, true);
    playerRender.addAnimation("borrow", 3, 0, 1, 1, true);
    playerRender.addAnimation("jump(down)", 2, 0, 3, 4, true);
    playerRender.addAnimation("jump(up)", 2, 10, 3, 4, true);
    playerRender.addAnimation("jump(horizontal)", 2, 5, 3, 4, true);
    playerRender.addAnimation("jumpAnticipation(down)", 2, 4, 1, 8, false);
    playerRender.addAnimation("jumpAnticipation(up)", 2, 14, 1, 8, false);
    playerRender.addAnimation("jumpAnticipation(horizontal)", 2, 9, 1, 8, false);
    playerRender.addAnimation("falling(down)", 3, 0, 1, 1, true);
    playerRender.addAnimation("falling(up)", 2, 10, 1, 1, true);
    playerRender.addAnimation("falling(horizontal)", 2, 7, 1, 1, true);
    playerRender.addAnimation("dive", 0, 0, 1, 1, true);
}

// gets two varibes && returns a normalized vector
Vector2 Player::Normalize(Vector2 oldDir)
{
    Vector2 normalVec;
    /*the length/mag of the vector is = sqrt(x^2 + y^2,
    for our movement direction this should
    always resualt in 1.41...*/
    float x = oldDir.x;
    float y = oldDir.y;
    float length = std::sqrt((x * x) + (y * y));
    /* the nomralized x and y of the vector are the
    x and y devidied by the length*/
    normalVec = {x / length, y / length};
    return normalVec;
}

void Player::Move(float speed)
{
    getDir();
    if (curSpeed < speed)
    {
        curSpeed += acc;
    }
    else
    {
        curSpeed = speed;
    }
    playerPos.x += (dir.x * curSpeed);
    playerPos.y += (dir.y * curSpeed);
}

void Player::getDir()
{
    if (IsKeyDown(KEY_D))
    {
        dir.x = 1;
        playerRender.flipped = false;
    }
    else if (IsKeyDown(KEY_A))
    {
        dir.x = -1;
        playerRender.flipped = true;
    }
    else
        dir.x = 0;

    if (IsKeyDown(KEY_W))
    {
        dir.y = -1;
    }
    else if (IsKeyDown(KEY_S))
    {
        dir.y = 1;
    }
    else
        dir.y = 0;

    if (playerState != IDLE)
    {
        if (dir.y == 1)
            renderDir = DOWN;
        else if (dir.y == -1)
            renderDir = UP;
        else if (dir.x == 1)
            renderDir = HORIZONTAL;
        else if (dir.x == -1)
            renderDir = HORIZONTAL;
    }

    // nomarlize the direction if moving diagonally
    if (dir.x != 0 && dir.y != 0)
    {
        dir = Normalize(dir);
    }
}

void Player::Draw()
{
    Vector2 drawPos = {playerPos.x, playerPos.y - zPos};
    render = {playerPos.x, playerPos.y - zPos, 32, 32};
    DrawRectangle(playerPos.x + 6, playerPos.y + 19, 12, 2, DARKGRAY);

    playerRender.position = drawPos;
    playerRender.Update();

    int row = 0;
    if (playerState == IDLE)
    {
        row = 0;
    }
    else if (playerState == WALKING)
    {
        row = 2;
    }
}

void Player::Colliding()
{
    playerPos.x -= (dir.x * curSpeed);
    playerPos.y -= (dir.y * curSpeed);
}

void Player::Update()
{
    switch (playerState)
    {
    case IDLE:
        if (animationState != jumpAnticipation)
        {
            if (IsKeyPressed(KEY_J) && grounded)
            {
                animationState = jumpAnticipation;
            }
            else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
            {
                playerState = WALKING;
            }
            else
            {
                animationState = idle;
            }
        }
        break;
    case WALKING:
        Move(walkSpeed);
        if (animationState != jumpAnticipation)
        {
            animationState = walking;
            if (IsKeyPressed(KEY_J) && grounded)
            {
                animationState = jumpAnticipation;
            }
            else if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
            {
                playerState = IDLE;
            }
        }

        break;
    case BURROWING:
        animationState = burrowing;
        if (IsKeyPressed(KEY_J) && grounded)
        {
            burrowCooldown.Reset();
            burrowCooldown.Start();
            playerState = JUMPING;
        }
        else if (IsKeyReleased(KEY_J) || burrowTimer.TimeOut())
        {
            burrowCooldown.Reset();
            burrowCooldown.Start();

            playerState = IDLE;
        }
        Move(burrowSpeed);
        break;
    case JUMPING:
        Move(walkSpeed);
        if (IsKeyPressed(KEY_J) && !burrowCooldown.running)
        {
            hangTimer.time = hangTimer.tarTime;
            playerState = DIVING;
        }

        if (zPos < 25.0f && !hangTimer.running)
        {
            animationState = jumping;
            zPos += jumpVel * GetFrameTime();
            if (zPos >= 25.0f)
            {
                zPos = 25.0f;
                grounded = false;
                hangTimer.Start();
            }
        }
        if (hangTimer.TimeOut())
            ;
        if (!grounded && !hangTimer.running)
        {
            if (!grounded)
            {
                animationState = falling;
                zPos -= 10;
            }
        }

        if (zPos <= 0)
        {
            zPos = 0;
            hangTimer.Reset();
            grounded = true;
            playerState = IDLE;
        }

        break;
    case DIVING:
        if (!grounded && !hangTimer.running)
        {
            if (!grounded)
            {
                zPos -= 10;
            }
        }
        if (zPos <= 0)
        {
            zPos = 0;
            hangTimer.Reset();
            grounded = true;

            burrowCooldown.Reset();
            burrowTimer.Start();
            playerState = BURROWING;
        }
        break;
    }
    std::cout << playerState << std::endl;

    burrowTimer.Update();
    burrowCooldown.Update();
    hangTimer.Update();
    playerRender.playAnimation(animationChart[animationState][renderDir]);

    if (animationState == jumpAnticipation && playerRender.Animations[animationChart[animationState][renderDir]].finished == true)
    {
        playerState = JUMPING;
    }

    if (burrowCooldown.TimeOut())
    {
        burrowTimer.time = 0.0f;
        burrowCooldown.time = 0.0f;
    }

    Draw();
}