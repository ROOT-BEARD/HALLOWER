#include <iostream>
#include <cmath>

#include "Player.h"
#include "Timer.h"
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
    // used mostly in place of a landing animation
    groundedTimer = Timer(0.05f);
    bufferAmount = 0.1;
    jumpBuffer = Timer(bufferAmount);
    attackArea = (Rectangle){playerPos.x, playerPos.y, 6, 6};
    attackActive = false;

    addAnimations();
}

// adds all the animations that the player will use
void Player::addAnimations()
{
    // *TEST ANIMATION*
    playerRender.addAnimation("spin", 0, 0, 4, 4, true);
    // walking
    playerRender.addAnimation("walk(up)", 1, 8, 4, 4, true);
    playerRender.addAnimation("walk(down)", 1, 0, 4, 4, true);
    playerRender.addAnimation("walk(horizontal)", 1, 4, 4, 4, true);
    // idle
    playerRender.addAnimation("idle(up)", 0, 2, 1, 1, true);
    playerRender.addAnimation("idle(down)", 0, 0, 1, 1, true);
    playerRender.addAnimation("idle(horizontal)", 0, 1, 1, 1, true);
    // burrowing
    playerRender.addAnimation("burrow", 3, 0, 1, 1, true);
    // jumping
    playerRender.addAnimation("jump(down)", 2, 0, 3, 4, true);
    playerRender.addAnimation("jump(up)", 2, 10, 3, 4, true);
    playerRender.addAnimation("jump(horizontal)", 2, 5, 3, 4, true);
    // falling
    playerRender.addAnimation("falling(down)", 2, 0, 1, 1, true);
    playerRender.addAnimation("falling(up)", 2, 10, 1, 1, true);
    playerRender.addAnimation("falling(horizontal)", 2, 7, 1, 1, true);
    // attack
    playerRender.addAnimation("attack(down)", 3, 0, 6, 12, false);
    playerRender.addAnimation("attack(up)", 3, 12, 6, 12, false);
    playerRender.addAnimation("attack(horizontal)", 3, 6, 6, 12, false);
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

// move the player with the top speed passed in as "speed"
void Player::Move(float speed)
{
    // gets the currentFrame time
    float delta = GetFrameTime();
    // gets the current directon inputs
    getDir();
    // if the curspeed is less than the top speed, increase it by the accleration * delta
    if (curSpeed < speed)
    {
        curSpeed += stats.acc * delta;
    }
    else
    {
        // other wise cap the curSpeed to the passed in top speed
        curSpeed = speed;
    }
    // move the play x and y postions by curSpeed * deltas
    playerPos.x += (dir.x * curSpeed * delta);
    playerPos.y += (dir.y * curSpeed * delta);
    collision = {playerPos.x + 9, playerPos.y + 14, 6, 6};
}

// gets the direction based on the inputs being pressed
void Player::getDir()
{
    // gets x direction depending on the key pressed
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

    /* gets the y direction depedning on the key pressed,
    NOTE: up direction is negative and down is positive*/
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

    /*only changes the render direction if not IDLE or not ATTACKING, so that
    way when idle you stay facing the last moving direction, and you cannot change
    the direction of your attack mid way through*/
    if (playerState != IDLE && playerState != ATTACKING)
    {
        if (dir.y == 1)
            renderDir = DOWN;
        else if (dir.y == -1)
            renderDir = UP;
        if (dir.x == 1)
        {
            renderDir = HORIZONTAL;
            playerRender.flipped = false;
        }
        else if (dir.x == -1)
        {
            renderDir = HORIZONTAL;
            playerRender.flipped = true;
        }
    }

    // nomarlize the direction if moving diagonally
    if (dir.x != 0 && dir.y != 0)
    {
        dir = Normalize(dir);
    }
}

// draw the player to the screen
void Player::Draw()
{
    /*where the player will be drawn to, the y position is subtracted
    by the zPos to allow player to jump*/
    Vector2 drawPos = {playerPos.x, playerPos.y - zPos};
    // the rectangle being used in place of a shadow sprite
    DrawRectangle(playerPos.x + 6, playerPos.y + 19, 12, 2, BLACK);
    // make the playerRender position = to the new drawPos
    playerRender.position = drawPos;
    // update the playerRender for animations
    playerRender.Update();
    DrawRectangleRec(collision, ColorAlpha(RED, 0.5f));
    if (playerState == ATTACKING)
    {
        if (attackActive)
            DrawRectangleRec(attackArea, ColorAlpha(GREEN, 0.5f));
        else
            DrawRectangleRec(attackArea, ColorAlpha(RED, 0.5f));
    }
}

// checks if the player should collide based off of states and tile flags
bool Player::ShouldCollide(const Tile &tile)
{
    if (!tile.solid)
    {
        return false;
    }
    if (playerState == BURROWING)
    {
        if (tile.burrowable)
            return false;
        return true;
    }
    if (playerState == JUMPING)
    {
        if (tile.jumpable)
        {
            return false;
        }
        return true;
    }
    return true;
}

/*call when player is colliding, push the player
in the oppisite direction that they are moving*/
void Player::Colliding()
{
    // update the collision shape position
    collision = {playerPos.x + 9, playerPos.y + 14, 6, 6};
    // loop for all the nearbyTiles
    for (Tile *tile : nearbyTiles)
    {
        // check for a collision
        if (CheckCollisionRecs(this->collision, tile->shape))
        {
            // get the overlap of the collision of the player and tile
            Rectangle overlap = GetCollisionRec(this->collision, tile->shape);
            // run if the overlap width is shallower than the height
            if (overlap.width < overlap.height)
            {
                // checks if the player should collide with the tile
                if (ShouldCollide(*tile))
                {
                    /* if collision is on the left side of the tile push the player
                    to the left side of the tile, otherwise, to the right*/
                    if (this->collision.x < tile->shape.x)
                        playerPos.x -= overlap.width;
                    else
                        playerPos.x += overlap.width;
                    // update the x position of the collision box
                    this->collision.x = playerPos.x + 9;
                }
            }
        }
    }
    collision = {playerPos.x + 9, playerPos.y + 14, 6, 6};
    // loop again for all nearbyTiles
    for (Tile *tile : nearbyTiles)
    {
        // check for collision
        if (CheckCollisionRecs(this->collision, tile->shape))
        {
            // check if should collide
            if (ShouldCollide(*tile))
            {
                // get overlap of the collision boxes
                Rectangle overlap = GetCollisionRec(this->collision, tile->shape);
                /*if the collision is at the top of the tile push it out to the top,
                otherwise, out to the bottom of the tile*/
                if (this->collision.y < tile->shape.y)
                    playerPos.y -= overlap.height;
                else
                    playerPos.y += overlap.height;
                // update the collision's y position
                this->collision.y = playerPos.y + 14;
            }
        }
    }
}

void Player::Jump()
{
    // chceck to see if jump key is pressed and grounded
    if (IsKeyPressed(KEY_J) && grounded)
    {
        // if so move to the JUMPING state
        playerState = JUMPING;
    }
}

void Player::Attack()
{
    if (IsKeyPressed(KEY_K))
    {
        playerState = ATTACKING;
    }
}

void Player::Update()
{
    // switch statement for players action state
    switch (playerState)
    {
    case IDLE:
        animationState = idle;
        Jump();
        Attack();
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_W) || IsKeyDown(KEY_S))
        {
            playerState = WALKING;
        }
        else
            curSpeed = 0.0f;
        break;
    case WALKING:
        Move(stats.walkSpeed);
        animationState = walking;
        Jump();
        Attack();
        if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && !IsKeyDown(KEY_W) && !IsKeyDown(KEY_S))
        {
            playerState = IDLE;
        }
        break;
    case BURROWING:
    {
        animationState = burrowing;
        Move(stats.burrowSpeed);
        bool jumpOut = true;
        if (!IsKeyDown(KEY_J) || burrowTimer.TimeOut())
        {
            for (Tile *tile : nearbyTiles)
            {
                if (CheckCollisionRecs(this->collision, tile->shape))
                {
                    if (tile->burrowable)
                    {
                        jumpOut = false;
                    }
                }
            }
            if (jumpOut)
            {
                burrowJump = true;
                playerState = JUMPING;
            }
        }
        break;
    }
    case JUMPING:
        if (burrowJump)
            Move(stats.walkSpeed * 2);
        else
            Move(stats.walkSpeed);

        if (IsKeyPressed(KEY_J))
        {
            jumpBuffer.Start();
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
            burrowJump = false;
            if (IsKeyDown(KEY_J))
            {
                burrowTimer.Start();
                playerState = BURROWING;
            }
            else if (jumpBuffer.running && !jumpBuffer.TimeOut())
            {
                jumpBuffer.Reset();
                animationState = jumping;
                zPos += stats.jumpVel * GetFrameTime();
            }
            else
            {
                playerState = IDLE;
            }
        }

        break;
    case ATTACKING:
    {
        Move(stats.walkSpeed);
        animationState = attacking;
        switch (renderDir)
        {
        case UP:
            attackArea = {playerPos.x + 8, playerPos.y, 8, 12};
            break;
        case DOWN:
            attackArea = {playerPos.x + 8, playerPos.y + 16, 8, 12};
            break;
        case HORIZONTAL:
            if (playerRender.flipped)
                attackArea = {playerPos.x, playerPos.y + 10, 12, 8};
            else
                attackArea = {playerPos.x + 16, playerPos.y + 10, 12, 8};
            break;

        default:
            break;
        }

        if (playerRender.curFrame == playerRender.Animations[playerRender.currentAnimation].startFrame + 3)
            attackActive = true;
        else if (playerRender.curFrame == playerRender.Animations[playerRender.currentAnimation].startFrame + 5)
            attackActive = false;
        if (playerRender.complete == true)
        {
            playerState = IDLE;
        }

        for (Tile *tile : nearbyTiles)
        {
            if (tile->breakable && CheckCollisionRecs(this->attackArea, tile->shape) && attackActive)
            {
                tile->OnHit();
            }
        }
        break;
    }
    }

    // updating timers
    burrowTimer.Update();
    hangTimer.Update();
    groundedTimer.Update();
    jumpBuffer.Update();

    Colliding();

    // play the current animation based off animation state and the current directon
    playerRender.playAnimation(animationChart[animationState][renderDir]);

    // makes the player grounded once timed out
    if (groundedTimer.TimeOut())
    {
        grounded = true;
        groundedTimer.Reset();
    }
}