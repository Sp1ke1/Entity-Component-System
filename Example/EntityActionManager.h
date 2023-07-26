#pragma once
#include "ECS/ObjectManager.h"
#include "ECS/ComponentBase.h"
#include "EntityAction.h"

class EntityActionManagerComponent : public ObjectManager<std::shared_ptr<EntityAction>> {

public:
    void GetAllVisibleActions ( std::vector<std::shared_ptr<EntityAction>> & out ) const
	{
		for ( auto & obj : m_Objects )
		{
			if ( obj && obj -> CanBeVisible() )
				out . push_back ( obj );
		}
	}
};