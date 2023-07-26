#pragma once

#include "ObjectManager.h"
#include "Entity.h"
#include <memory>


/**
 * @class EntityManager
 * @brief Manages entities in the Entity-Component-System (ECS) framework.
 *
 * The EntityManager class is responsible for creating, storing, and retrieving entities in the system.
 * It uses an ObjectManager to store and manage entities efficiently.
 * The class provides both a fast regular interface and a safe interface with additional checks.
 */
class EntityManager
{

public:

	/* Begin EntityManager interface */

	/**
     * @brief Creates a new entity and adds it to the manager.
     * @return A handle to the newly created entity.
     */
	EntityHandle CreateEntity ()
	{
		Entity e ( m_Entities . GetNextHandle () );
		return m_Entities . AddObject ( e );
	}

	/**
     * @brief Removes an entity from the manager.
     * @param entityHandle The handle of the entity to be removed.
     */
	void RemoveEntity ( EntityHandle entityHandle )
	{
		m_Entities . RemoveObject ( entityHandle );
	}

	/**
     * @brief Retrieves a reference to the entity associated with the given handle without any checks
     * @param entityHandle The handle of the entity to retrieve.
     * @return A reference to the entity
     */
	Entity & GetEntity ( EntityHandle entityHandle )
	{
		return m_Entities . GetObject ( entityHandle );
	}

	/**
     * @brief Removes all entities from the manager.
     */
	void RemoveAllEntities ()
	{
		m_Entities . Clear ();
	}

	/**
     * @brief Checks if the given entity handle is valid.
     * @param entityHandle The handle of the entity to check.
     * @return True if the handle is valid, false otherwise.
     */
	bool GetIsValidEntityHandle ( EntityHandle entityHandle ) const
	{
		return m_Entities . GetIsValidHandle ( entityHandle );
	}

	/* End EntityManager interface */


	/* Begin EntityManager safe interface */

	/**
     * @brief Retrieves a reference to the entity associated with the given handle with additional safety checks.
     * @param entityHandle The handle of the entity to retrieve.
     * @return An optional containing a reference to the entity if it exists, or an empty optional if not found.
     */
	std::optional <std::reference_wrapper <Entity>> GetEntityChecked ( EntityHandle entityHandle )
	{
		return m_Entities . GetObjectChecked ( entityHandle );
	}

	/**
     * @brief Removes an entity from the manager with additional safety checks.
     * @param entityHandle The handle of the entity to be removed.
     * @return True if the entity was removed successfully, false otherwise (e.g., handle not found).
     */
	bool RemoveEntityChecked ( EntityHandle entityHandle )
	{
		return m_Entities . RemoveObjectChecked ( entityHandle );
	}

	/* End EntityManager safe interface */

private:

	ObjectManager <Entity> m_Entities; /**< The ObjectManager to store and manage entities. */
};

