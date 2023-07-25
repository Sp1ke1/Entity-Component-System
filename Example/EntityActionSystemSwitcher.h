#pragma once

#include "ECS/ECS.h"
#include "EntityActionManager.h"


class EntityActionSystemSwitcher : public System

{
public:
	virtual void Run ( ECS & ecs ) override
	{
		for ( auto handle : m_Entities )
		{
			auto & actionManager = ecs. GetComponent <EntityActionManagerComponent>( handle );
			for ( const auto & action : actionManager )
			{
				if ( action )
					action -> SetVisible ( ! action -> CanBeVisible() );
			}
		}
	}
};