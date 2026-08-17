#include <iostream>
#include <cmath>

#include "Player.h"
#include "Timer.h"
#include "AnimatedSprite.h"

// constructor
Player::Player()
{
    animationState = idle;
    renderDir = DOWN;
    playerState = IDLE;
    playerRender = AnimatedSprite("Art/playerSheet.png", Vector2{24, 24});
    shadowTexture = LoadTexture("Art/shadow.png");
    dir = Vector2{0.0f, 0.0f};
    zPos = 0;
    playerPos = Vector2{32.0f, 32.0f};
    curSpeed = 0.0f;
    burrowTimer = Timer(stats.burrowTime);
    hangTimer = Timer(stats.hangTime);
    grounded = true;
    burrowJump = false;
    groundedTimer = Timer(0.05f);
    bufferAmount = 0.1;
    jumpBuffer = Timer(bufferAmount);
    attackArea = {playerPos.x, playerPos.y, 6, 6};
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
    playerRender.addAnimation("burrow", 5, 0, 6, 12, true);
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
    // falling
    playerRender.addAnimation("falling(pit)", 4, 0, 12, 8, false);
    // sliding
    playerRender.addAnimation("sliding", 4, 0, 1, 8, false);
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
void Player::Move(float speed, float delta)
{
    // gets the current directon inputs
    getDir();
    // checks to see if you are moving in at least one direction
    if (dir.x != 0 || dir.y != 0)
    {
        // increases speed by accereation * delta
        curSpeed += stats.acc * delta;
        // caps the speed to the top speed passed through
        if (curSpeed >= speed)
            curSpeed = speed;
    }

    // move the play x and y postions by curSpeed * delta
    playerPos.x += (dir.x * curSpeed * delta);
    playerPos.y += (dir.y * curSpeed * delta);
    // update the collision rect
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
    float roundedX = std::round(playerPos.x);
    float roundedY = std::round(playerPos.y);
    Vector2 drawPos = {roundedX, roundedY - zPos};
    if (playerState != FALLINGPIT && playerState != BURROWING)
    {
        // the rectangle being used in place of a shadow sprite
        DrawTexture(shadowTexture, roundedX + 7, roundedY + 16, WHITE);
    }
    // make the playerRender position = to the new drawPos
    playerRender.position = drawPos;
    // update the playerRender for animations
    playerRender.Update();

    /* the attack and player collision boxes, used for debugging
    DrawRectangleRec(collision, ColorAlpha(RED, 0.5f));
    if (playerState == ATTACKING)
    {
        if (attackActive)
            DrawRectangleRec(attackArea, ColorAlpha(GREEN, 0.5f));
        else
            DrawRectangleRec(attackArea, ColorAlpha(RED, 0.5f));
    }*/
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
    }
    if (playerState == JUMPING)
    {
        if (tile.jumpable)
            return false;
    }
    return true;
}

/*used to check for collisions, and if so, updates the players position*/
void Player::Collide()
{
    // if the player is sliding return
    if (playerState == SLIDING)
    {
        return;
    }
    // update the collision shape position
    collision = {playerPos.x + 9, playerPos.y + 14, 6, 6};
    // loop for all the nearbyTiles
    for (Tile *tile : nearbyTiles)
    {
        // check for a collision
        if (CheckCollisionRecs(this->collision, tile->shape))
        {
            // the the player runs into a pit change state and return
            if (tile->type == Tile::PIT && playerState != JUMPING)
            {
                playerState = FALLINGPIT;
                return;
            }
            // checks if the player should collide with the tile
            if (ShouldCollide(*tile))
            {
                // get the overlap of the collision of the player and tile
                Rectangle overlap = GetCollisionRec(this->collision, tile->shape);
                /*if landing on a jumpable tile and collision completley overlaps, make the player slide back to the last saved
                safe spot, and return*/
                if (tile->jumpable && groundedTimer.running && overlap.width >= overlap.height && overlap.height >= overlap.width)
                {
                    playerState = SLIDING;
                    return;
                }
                // if the overlap width is shallower then the height, resolve the x axis
                if (overlap.width <= overlap.height)
                {
                    // get the center of both tile and player collision
                    float playerCenterX = this->collision.x + (this->collision.width / 2.0f);
                    float tileCenterX = tile->shape.x + (tile->shape.width / 2.0f);
                    /* if collision is on the left side of the tile push the player
                    to the left side of the tile, otherwise, to the right*/
                    if (playerCenterX < tileCenterX)
                        playerPos.x -= overlap.width;
                    else
                        playerPos.x += overlap.width;
                    // update the x position of the collision box
                    this->collision.x = playerPos.x + 9;
                }
            }
        }
        // check to see if collsion was resolved, and if not run for the y-axis
        if (CheckCollisionRecs(this->collision, tile->shape))
        {
            Rectangle overlap = GetCollisionRec(this->collision, tile->shape);
            // check to see if height is more shallow
            if (overlap.height < overlap.width)
            {
                // check if should collide
                if (ShouldCollide(*tile))
                {
                    // get center points
                    float playerCenterY = this->collision.y + (this->collision.height / 2.0f);
                    float tileCenterY = tile->shape.y + (tile->shape.height / 2.0f);
                    /*if the collision is at the top of the tile push it out to the top,
                    otherwise, out to the bottom of the tile*/
                    if (playerCenterY < tileCenterY)
                        playerPos.y -= overlap.height;
                    else
                        playerPos.y += overlap.height;
                    // update the collision's y position
                    this->collision.y = playerPos.y + 14;
                }
            }
        }
    }
}

// checks nearby tiles, and returns wheter or not a vaild collision is being made
bool Player::IsColliding()
{
    for (Tile *tile : nearbyTiles)
    {
        if (ShouldCollide(*tile) && CheckCollisionRecs(this->collision, tile->shape))
        {
            return true;
        }
    }
    return false;
}

void Player::Jump()
{
    // chceck to see if jump key is pressed and grounded
    if (IsKeyPressed(KEY_J) && grounded)
    {
        // if so move to the JUMPING state
        playerState = JUMPING;
        // save the safe positon
        lastPos = playerPos;
    }
}

void Player::Attack()
{
    if (IsKeyPressed(KEY_K))
    {
        playerState = ATTACKING;
    }
}

void Player::Update(float delta)
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
        Move(stats.walkSpeed, delta);
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
        Move(stats.burrowSpeed, delta);
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
            Move(stats.walkSpeed * 2, delta);
        else
            Move(stats.walkSpeed, delta);

        if (IsKeyPressed(KEY_J))
        {
            jumpBuffer.Start();
        }

        if (zPos < stats.jumpHeight && animationState != falling)
        {
            animationState = jumping;
            zPos += stats.jumpVel * delta;
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
            zPos -= stats.gravity * delta;
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
                zPos += stats.jumpVel * delta;
            }
            else
            {
                playerState = IDLE;
            }
        }

        break;
    case ATTACKING:
    {
        Move(stats.walkSpeed, delta);
        animationState = attacking;
        switch (renderDir)
        {
        case UP:
            attackArea = {playerPos.x + 10, playerPos.y + 4, 4, 10};
            break;
        case DOWN:
            attackArea = {playerPos.x + 10, playerPos.y + 12, 4, 10};
            break;
        case HORIZONTAL:
            if (playerRender.flipped)
                attackArea = {playerPos.x, playerPos.y + 10, 10, 4};
            else
                attackArea = {playerPos.x + 12, playerPos.y + 10, 10, 4};
            break;

        default:
            break;
        }

        if (playerRender.curFrame == playerRender.Animations[playerRender.currentAnimation].startFrame + 3)
            attackActive = true;
        else if (playerRender.curFrame == playerRender.Animations[playerRender.currentAnimation].startFrame + 4)
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
    case FALLINGPIT:
        animationState = fallingpit;
        if (playerRender.complete && playerRender.currentAnimation == "falling(pit)")
        {
            playerPos = {1 * 16, 2 * 16};
            playerState = IDLE;
        }
        break;
    case SLIDING:
    {
        animationState = sliding;
        Vector2 direction = {lastPos.x - playerPos.x, lastPos.y - playerPos.y};
        Vector2 nomarlizedDir = Normalize(direction);
        float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);

        if (distance < 1.0f || !IsColliding())
        {
            playerState = IDLE;
        }
        else
        {
            playerPos.x += nomarlizedDir.x * 100 * delta;
            playerPos.y += nomarlizedDir.y * 100 * delta;
            collision = {playerPos.x + 9, playerPos.y + 14, 6, 6};
        }
        break;
    }
    }

    // updating timers
    burrowTimer.Update();
    hangTimer.Update();
    groundedTimer.Update();
    jumpBuffer.Update();

    Collide();

    // play the current animation based off animation state and the current directon
    playerRender.playAnimation(animationChart[animationState][renderDir]);

    // makes the player grounded once timed out
    if (groundedTimer.TimeOut())
    {
        grounded = true;
        groundedTimer.Reset();
    }
}