#pragma once

#include "ObjectManager.h"
#include "Entity.h"

class EntityManager
{

public:

	/* Begin EntityManager interface */
	EntityHandle CreateEntity ()
	{
		Entity e ( m_Entities . GetNextHandle () );
		return m_Entities . AddObject ( e );
	}

	void RemoveEntity ( EntityHandle entityHandle )
	{
		m_Entities . RemoveObject ( entityHandle );
	}

	Entity & GetEntity ( EntityHandle entityHandle )
	{
		return m_Entities . GetObject ( entityHandle );
	}

	void RemoveAllEntities ()
	{
		m_Entities . Clear ();
	}

	bool GetEntityHasComponent ( EntityHandle handle, ComponentType componentType )
	{
		auto obj = GetEntityChecked ( handle );
		if ( ! obj . has_value () )
			return false;
		return obj . value () . get () . GetHasComponent ( handle );
	}

	bool GetIsValidEntityHandle ( EntityHandle entityHandle ) const
	{
		return m_Entities . GetIsValidHandle ( entityHandle );
	}

	/* End EntityManager interface */


	/* Begin EntityManager safe interface */
	std::optional <std::reference_wrapper <Entity>> GetEntityChecked ( EntityHandle entityHandle )
	{
		return m_Entities . GetObjectChecked ( entityHandle );
	}

	bool RemoveEntityChecked ( EntityHandle entityHandle )
	{
		return m_Entities . RemoveObjectChecked ( entityHandle );
	}
	/* End EntityManager safe interface */

private:

	ObjectManager <Entity> m_Entities;
};

