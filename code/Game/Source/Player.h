#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Input.h"
#include "Render.h"
#include "Animation.h"

#include "Point.h"

#include "SDL/include/SDL.h"

#define INIT_COMBAT_POSX 225
#define INIT_COMBAT_POSY 400
#define SPRITE_TILE_WIDTH 154
#define SPRITE_TILE_HEIGHT 141


class Animation;

class Player : public Entity
{
public:

    Player();

    bool Update(float dt);

    bool Draw();

    void SetTexture(SDL_Texture *tex);

    void Jump();

    void FeatherJump();

    void Crouch();

    SDL_Rect GetCombatBounds()
    {
        return colliderCombat;
    }

    int GetCombatWidth() const
    {
        return colliderCombat.w;
    }

    int GetCombatHeight() const
    {
        return colliderCombat.h;
    }

    int GetLiveWidth() const
    {
        return colliderLive.w;
    }

    int GetLiveHeight() const
    {
        return colliderLive.h;
    }

public:
    Animation cIdleAnim;
    Animation cPos0AttackAnim;
    Animation cStepAnim;
    Animation cJumpAnim;
    Animation cDieAnim;
    Animation cCrouchAnim;

    bool jump = false;
    bool crouch = false;
    int jumpTime = 0;
    int jumpFeatherTime = 0;
    int crouchTime = 0;

public:
    friend class Combat;

    int health = 35;
    int maxHealth = 35;
    int defense = 5;
    int strength = 30;
    int velocity = 0;
    int luck = 0;
    int stab;
    
    int lvl;
    int exp;
    int money;

public:
    SDL_Texture* texture;

    iPoint positionLive;
    int positionCombat;

    SDL_Rect colliderLive;
    SDL_Rect colliderCombat;
};

#endif // __PLAYER_H__
