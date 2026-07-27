#include "Player.h"
#include "Timer.h"
#include "iostream"
#include "cmath"
#include "AnimatedSprite.h"

Player::Player()
{
    animationState = idle;
    renderDir = DOWN;
    playerState = IDLE;
    playerRender = AnimatedSprite("Art/playerSheet.png", Vector2{24, 24});
    dir = Vector2{0.0f, 0.0f};
    zPos = 0;
    playerPos = Vector2{32.0f, 32.0f};
    curSpeed = 0.0f;
    burrowTimer = Timer(stats.burrowTime);
    hangTimer = Timer(stats.hangTime);
    grounded = true;
    burrowJump = false;
    //used mostly in place of a landing animation
    groundedTimer = Timer(0.05f);
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
    playerRender.addAnimation("burrow", 3, 0, 1, 1, true);
    playerRender.addAnimation("jump(down)", 2, 0, 3, 4, true);
    playerRender.addAnimation("jump(up)", 2, 10, 3, 4, true);
    playerRender.addAnimation("jump(horizontal)", 2, 5, 3, 4, true);
    playerRender.addAnimation("falling(down)", 2, 0, 1, 1, true);
    playerRender.addAnimation("falling(up)", 2, 10, 1, 1, true);
    playerRender.addAnimation("falling(horizontal)", 2, 7, 1, 1, true);
    playerRender.addAnimation("dive", 2, 2, 1, 1, false);
}

// gets two varibes && returns a normalized vector
Vector2 Player::Normalize(const Vector2 &oldDir) const
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
    // gets the currentFrame time
    float delta = GetFrameTime();
    // gets the current directon inputs
    getDir();
    //if the curspeed is less than the top speed, increase it by the accleration * delta
    if (curSpeed < speed)
    {
        curSpeed += stats.acc * delta;
    }
    else
    {
        // other wise cap the curSpeed to the passed in top speed
        curSpeed = speed;
    }
    playerPos.x += (dir.x * curSpeed * delta);
    playerPos.y += (dir.y * curSpeed * delta);
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
        else if (dir.x == 1 or dir.x == -1)
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
    DrawRectangle(playerPos.x + 6, playerPos.y + 19, 12, 2, BLACK);

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
        animationState = idle;
        if (IsKeyPressed(KEY_J) && grounded)
        {
            playerState = JUMPING;
        }
        else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
        {
            playerState = WALKING;
        }
        else
            curSpeed = 0.0f;
        break;
    case WALKING:
        Move(stats.walkSpeed);
        animationState = walking;
        if (IsKeyPressed(KEY_J) && grounded)
        {
            playerState = JUMPING;
        }
        else if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
        {
            playerState = IDLE;
        }
        break;
    case BURROWING:
        animationState = burrowing;
        Move(stats.burrowSpeed);
        if (IsKeyReleased(KEY_J) || burrowTimer.TimeOut())
        {
            burrowJump = true;
            playerState = JUMPING;
        }
        break;
    case JUMPING:
        if (burrowJump) Move(stats.walkSpeed * 2);
        else Move(stats.walkSpeed);
        if (IsKeyPressed(KEY_J) && !burrowJump)
        {
            playerState = DIVING;
            break;
        }

        if (zPos < stats.jumpHeight && animationState != falling)
        {
            animationState = jumping;
            zPos += stats.jumpVel * GetFrameTime();
            if (zPos >= stats.jumpHeight)
            {
                zPos = stats.jumpHeight;
                grounded = false;
                hangTimer.Start();
            }
        }

        if (!grounded && hangTimer.TimeOut())
        {
            animationState = falling;
            zPos -= stats.gravity * GetFrameTime();
        }

        if (zPos <= 0)
        {
            zPos = 0;
            hangTimer.Reset();
            groundedTimer.Start();
            playerState = IDLE;
            burrowJump = false;
        }

        break;
    case DIVING:
        animationState = diving;
        Move(stats.walkSpeed * 1.5);    

        zPos -= 200.0 * GetFrameTime();

        if (zPos <= 0)
        {
            zPos = 0;
            hangTimer.Reset();

            burrowTimer.Start();
            playerState = BURROWING;
        }
        break;
    }

    burrowTimer.Update();
    hangTimer.Update();
    groundedTimer.Update();

    playerRender.playAnimation(animationChart[animationState][renderDir]);

    if(groundedTimer.TimeOut()){
        grounded = true;
        groundedTimer.Reset();
    }
    std::cout<<groundedTimer.time<<std::endl;

    Draw();
}