#ifndef __ENTITY_H__
#define __ENTITY_H__

class Enemy;
class Item;

enum class EntityType
{
    PLAYER,
    ENEMY,
    ITEM,
    MAP,
    UNKNOWN
};

class Entity
{
public:

    Entity(EntityType type) : type(type), active(true) {}

    virtual bool Update(float dt)
    {
        return true;
    }

public:

    EntityType type;
    bool active = true;
    bool renderable = false;
};

#endif // __ENTITY_H__