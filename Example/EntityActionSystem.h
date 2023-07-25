#pragma once

#include "System.h"
#include "ECS/ECS.h"
#include "EntityActionManager.h"


class EntityActionSystem : public System

{
public:
	virtual void Run ( ECS & ecs ) override
	{
		for ( auto handle : m_Entities )
		{
			auto & actionManager = ecs. GetComponent <EntityActionManagerComponent>( handle );
			std::vector <std::shared_ptr<EntityAction>> visibleActions;
			actionManager . GetAllVisibleActions( visibleActions );
			for ( auto & action : visibleActions )
			{
				action -> Action();
			}
		}
	}
};