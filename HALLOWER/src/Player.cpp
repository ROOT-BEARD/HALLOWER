#include "Player.h"
#include "Timer.h"
#include "iostream"
#include "cmath"

Player::Player()
{
    renderDir = Vector2{0.0f, 0.0f};
    dir = Vector2{0.0f, 0.0f};
    zPos = 0;
    playerPos = Vector2{32.0f, 32.0f};
    burrowTime = 2.0f; // two seconds
    cooldown = 1.25f;
    curSpeed = 0.0f;
    burrowed = false;
    acc = 0.25f;
    walkSpeed = 5.0f;
    burrowSpeed = 10.0f;
    topSpeed = 10.0f;
    walking = false;
    render = {playerPos.x, playerPos.y, 32, 32}; // used to hold what character the player is
    burrowTimer = Timer(burrowTime);
    burrowCooldown = Timer(cooldown);
    hangTime = 0.5f;
    hangTimer = Timer(hangTime);
    grounded = true;
    jumpVel = 250.0f;
    jumping = false;
    playerState = IDLE;
    playerRender = LoadTexture("Art/characterSheet.png");
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
    }
    else if (IsKeyDown(KEY_A))
    {
        dir.x = -1;
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

    if (dir.x != 0 && dir.y != 0)
    {
        dir = Normalize(dir);
    }
    if (dir.x != 0 || dir.y != 0)
    {
        renderDir = dir;
    }
}

void Player::Draw()
{
    int mul = 0;
    if (renderDir.x < -0.05f)
    {
        mul = 6;
    }
    else if (renderDir.x > 0.05f)
    {
        mul = 2;
    }
    else if (renderDir.y > 0.05f)
    {
        mul = 0;
    }
    else if (renderDir.y < -0.05f)
    {
        mul = 4;
    }

    Vector2 drawPos = {playerPos.x, playerPos.y - zPos};
    render = {playerPos.x, playerPos.y - zPos, 32, 32};
    DrawRectangle((int)playerPos.x + 22, (int)playerPos.y + 72, 32, 4, BLACK);
    // DrawRectangleRec(render, playerState == BURROWING ? BLACK : ORANGE);
    int row = 0;
    if (playerState == IDLE)
    {
        row = 0;
    }
    else if (playerState == WALKING)
    {
        row = 2;
    }
    Rectangle source = {(float)(72 * mul), (float)(72 * row), 72.0f, 72.0f};
    DrawTextureRec(playerRender, source, drawPos, WHITE);
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
        if (IsKeyPressed(KEY_J) && grounded)
        {
            playerState = JUMPING;
        }
        else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
        {
            playerState = WALKING;
        }
        break;
    case WALKING:
        Move(walkSpeed);
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

    if (burrowCooldown.TimeOut())
    {
        burrowTimer.time = 0.0f;
        burrowCooldown.time = 0.0f;
    }

    Draw();
}