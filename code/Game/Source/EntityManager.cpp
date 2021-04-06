#include "App.h"
#include "EntityManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Item.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{

}

EntityManager::~EntityManager()
{}

bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	return ret;
}

bool EntityManager::CleanUp()
{
	if (!active) return true;

	return true;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* ret = nullptr;

	switch (type)
	{
		case EntityType::PLAYER: ret = new Player();  break;
		case EntityType::ENEMY: ret = new Enemy();  break;
		case EntityType::ITEM: ret = new Item();  break;
		default: break;
	}

	if (ret != nullptr && type == EntityType::ENEMY) enemies.Add((Enemy*)ret);
	else if (ret != nullptr && type == EntityType::ITEM) items.Add((Item*)ret);

	return ret;
}

bool EntityManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool EntityManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic)
	{
		// TODO: Update all entities 
	}

	return true;
}
