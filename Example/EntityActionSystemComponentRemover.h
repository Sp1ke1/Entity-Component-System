#pragma once
#include "ECS/ECS.h"
#include "PrivateEntityAction.h"
#include "EntityActionManager.h"

class EntityActionSystemComponentRemover : public System
{
public:
	virtual void Run ( ECS & ecs ) override
	{
		for ( auto entityHandle : m_Entities )
		{
			auto & actionManager = ecs . GetComponent <EntityActionManagerComponent>( entityHandle );
			for ( std::size_t i = 0; i < actionManager.Size(); i ++  )
			{
				if ( std::dynamic_pointer_cast<PrivateEntityAction> ( actionManager . GetObjectByIndex ( i ) ) )
					actionManager . RemoveObjectByIndex ( i );
			}
		}
	}
};