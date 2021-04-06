#include "Player.h"
#include "App.h"

#include "Scene.h"
#include "Combat.h"

#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
    texture = NULL;

    colliderCombat = {INIT_COMBAT_POSX, INIT_COMBAT_POSY, 48, 88};

    // Define Player animations
    for (int i = 0; i < 6; i++)
    {
        cIdleAnim.PushBack({ SPRITE_TILE_WIDTH*i,0 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
    }
    cIdleAnim.speed = 0.120f;

    for (int i = 0; i < 17; i++)
    {
        cPos0AttackAnim.PushBack({ 0,2256 + (i*720),1280,720 });
    }
    cPos0AttackAnim.speed = 0.190f;

    for (int i = 0; i < 8; i++)
    {
        cJumpAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT*5 ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
       
    }
    cJumpAnim.speed = 0.1f;

    for (int i = 0; i < 6; i++)
    {
        cStepAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT ,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
    }
    cStepAnim.speed = 0.190f;

    for (int i = 0; i < 10; i++)
    {

        cDieAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT *2,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });
        
    }
    cDieAnim.speed = 0.08f;
    cDieAnim.loop = false;

    for (int i = 0; i < 6; i++)
    {

        cCrouchAnim.PushBack({ SPRITE_TILE_WIDTH * i,SPRITE_TILE_HEIGHT * 6,SPRITE_TILE_WIDTH,SPRITE_TILE_HEIGHT });

    }
    cCrouchAnim.speed = 0.1f;
    cCrouchAnim.loop = false;
}

bool Player::Update(float dt)
{
    return true;
}

bool Player::Draw()
{
    return false;
}

void Player::SetTexture(SDL_Texture *tex)
{
    texture = tex;
}

void Player::Jump()
{
    if (jumpTime < 34)
    {
        colliderCombat.y -= 17;
        colliderCombat.y += jumpTime;
        jumpTime++;
    }
    else
    {
        colliderCombat.y = INIT_COMBAT_POSY;
        jumpTime = 0;
        jump = false;
        app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
    }
}

void Player::FeatherJump()
{
    if (jumpFeatherTime <= 17)
    {
        colliderCombat.y -= 17;
        colliderCombat.y += jumpFeatherTime;
        jumpFeatherTime++;
    }
    else if (jumpFeatherTime > 17 && jumpFeatherTime < 70)
    {
        colliderCombat.y -= (17 + jumpFeatherTime - 20);
        colliderCombat.y += jumpFeatherTime;
        jumpFeatherTime++;
    }
    else
    {
        colliderCombat.y = INIT_COMBAT_POSY;
        jumpFeatherTime = 0;
        jump = false;
        app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
    }
}

void Player::Crouch()
{
    if (crouchTime < 40)
    {
        if (crouchTime == 0)
        {
            colliderCombat.y += 40;
            colliderCombat.h = 48;
        }
        crouchTime++;
    }
    else
    {
        colliderCombat.y = INIT_COMBAT_POSY;
        colliderCombat.h = 88;
        crouchTime = 0;
        crouch = false;
        app->scene->combatScene->currentPlayerAnim = &cIdleAnim;
    }
}
