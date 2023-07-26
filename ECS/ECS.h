#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"


/**
 * @class ECS
 * @brief Entity Component System (ECS) manager.
 *
 * The ECS class serves as the central manager for the Entity Component System framework.
 * It provides an interface for creating, removing, and accessing entities and their components.
 * The class also allows registering and running systems within the ECS framework.
 * It provides both a regular interface and a safe interface with additional checks.
 */
class ECS
{

public:

	// Begin Entity Component System interface

	/**
     * @brief Creates a new entity.
     * @return The handle of the newly created entity.
     */
	EntityHandle CreateEntity ()
	{
		return m_EntityManager . CreateEntity ();
	}

	/**
     * @brief Removes an entity from the ECS.
     * @param entityHandle The handle of the entity to be removed.
     */
	void RemoveEntity ( EntityHandle entityHandle )
	{
		Entity & e = m_EntityManager . GetEntity ( entityHandle );
		m_ComponentManager . OnEntityRemoved ( e );
		m_SystemManager . OnEntityRemoved ( e );
		m_EntityManager . RemoveEntity ( entityHandle );
	}

	/**
     * @brief Retrieves a reference to the entity associated with the given handle.
     * @param entityHandle The handle of the entity to retrieve.
     * @return A reference to the entity.
     */
	Entity & GetEntity ( EntityHandle entityHandle )
	{
		return m_EntityManager . GetEntity ( entityHandle );
	}

	/**
     * @brief Checks if an entity has a component of the specified type.
     * @tparam T The type of the component to check for.
     * @param entityHandle The handle of the entity to check.
     * @return True if the entity has the component, false otherwise.
     */
	template <typename T>
	bool GetEntityHasComponent ( EntityHandle entityHandle )
	{
		const auto ComponentType = GetComponentType <T> ();
		Entity & e = GetEntity ( entityHandle );
		return e . GetHasComponent ( ComponentType );
	}

	/**
     * @brief Checks if an entity handle is valid.
     * @param entityHandle The handle of the entity to check.
     * @return True if the entity handle is valid, false otherwise.
     */
	bool GetIsValidEntityHandle ( EntityHandle entityHandle ) const
	{
		return m_EntityManager . GetIsValidEntityHandle ( entityHandle );
	}

	/**
     * @brief Registers a new component type with the ECS.
     * @tparam T The type of the component to be registered.
     */
	template <typename T>
	void RegisterComponent ()
	{
		return m_ComponentManager . RegisterComponent <T> ();
	}

	/**
     * @brief Adds a new component to an entity.
     * @tparam T The type of the component to be added.
     * @param entityHandle The handle of the entity to which the component will be added.
     * @param component The component to be added.
     */
	template <typename T>
	void AddComponent ( EntityHandle entityHandle, T && component )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . AddComponent ( e, std::forward <T> ( component ) );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	/**
     * @brief Adds a new component to an entity.
     * @tparam T The type of the component to be added.
     * @param entityHandle The handle of the entity to which the component will be added.
     * @param component The component to be added.
     */
	template <typename T>
	void AddComponent ( EntityHandle entityHandle, const T & component )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . AddComponent ( e, component );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	/**
     * @brief Removes a component of the specified type from an entity.
     * @tparam T The type of the component to be removed.
     * @param entityHandle The handle of the entity from which the component will be removed.
     */
	template <typename T>
	void RemoveComponent ( EntityHandle entityHandle )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . RemoveComponent <T> ( e );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	/**
     * @brief Retrieves a reference to the component of the specified type associated with the given entity.
     * @tparam T The type of the component to retrieve.
     * @param entity The handle of the entity for which to retrieve the component.
     * @return A reference to the component.
     */
	template <typename T>
	T & GetComponent ( EntityHandle entity )
	{
		Entity & e = GetEntity ( entity );
		return m_ComponentManager . GetComponent <T> ( e );
	}
	/**
     * @brief Retrieves the ComponentType code for the specified component type.
     * @tparam T The type of the component to get the ComponentType code for.
     * @return The code representing the ComponentType of the specified component.
     */
	template <typename T>
	ComponentType GetComponentType ()
	{
		return m_ComponentManager . GetComponentType <T> ();
	}

	/**
     * @brief Retrieves a weak pointer to the component array of the specified type.
     * @tparam T The type of the component to retrieve.
     * @return A weak pointer to the component array if registered, otherwise an expired weak pointer.
     */
	template <typename T>
	std::weak_ptr <ObjectManager <T>> GetComponentsByType ()
	{
		return m_ComponentManager . GetComponentsByType <T> ();
	}

	/**
     * @brief Checks if a component of the specified type is registered with the ECS.
     * @tparam T The type of the component to check for registration.
     * @return True if the component is registered, false otherwise.
     */
	template <typename T>
	bool GetIsComponentRegistered () const
	{
		return m_ComponentManager . GetIsComponentRegistered <T> ();
	}

	/**
     * @brief Registers a new system with the ECS.
     * @tparam T The type of the system to be registered.
     * @tparam ComponentTypes The component types required by the system.
     */
	template <typename T, typename ... ComponentTypes>
	void RegisterSystem ()
	{
		return m_SystemManager . RegisterSystem <T> ( std::move ( GetComponentsSignature <ComponentTypes ...> () ) );
	}

	/**
     * @brief Runs a system of the specified type.
     * @tparam T The type of the system to be run.
     */
	template <typename T>
	void RunSystem ()
	{
		m_SystemManager . RunSystem <T> ( * this );
	}

	/**
     * @brief Retrieves a weak pointer to the system of the specified type.
     * @tparam T The type of the system to retrieve.
     * @return A weak pointer to the system if registered, otherwise an expired weak pointer.
     */
	template <typename T>
	std::weak_ptr <T> GetSystem ()
	{
		return m_SystemManager . GetSystem <T> ();
	}

	/**
     * @brief Retrieves the SystemType code for the specified system type.
     * @tparam T The type of the system to get the SystemType code for.
     * @return The code representing the SystemType of the specified system.
     */
	template <typename T>
	SystemType GetSystemType () const
	{
		return m_SystemManager . GetSystemType <T> ();
	}

	/**
     * @brief Checks if a system of the specified type is registered with the ECS.
     * @tparam T The type of the system to check for registration.
     * @return True if the system is registered, false otherwise.
     */
	template <typename T>
	bool GetIsSystemRegistered () const
	{
		return m_SystemManager . GetIsSystemRegistered <T> ();
	}

	/**
     * @brief Retrieves the signature representing the required components for a system.
     * @tparam ComponentTypes The component types required by the system.
     * @return The signature representing the required components for the system.
     */
	template <typename ... ComponentTypes>
	Signature GetComponentsSignature () const
	{
		return { m_ComponentManager . GetComponentType <ComponentTypes> () ... };
	}
	/* End Entity Component System interface */


	/* Begin Entity Component System safe interface */

	/**
     * @brief Retrieves a safe optional reference to the entity associated with the given handle.
     * @param entityHandle The handle of the entity to retrieve.
     * @return A safe optional reference to the entity, or an empty optional if the handle is invalid.
     */
	std::optional <std::reference_wrapper <Entity>> GetEntityChecked ( EntityHandle entityHandle )
	{
		return m_EntityManager . GetEntityChecked ( entityHandle );
	}

	/**
     * @brief Removes an entity from the ECS in a safe manner.
     * @param entityHandle The handle of the entity to be removed.
     * @return True if the entity was successfully removed, false if the entity handle is invalid.
     */
	bool RemoveEntityChecked ( EntityHandle entityHandle )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		RemoveEntity ( entityHandle );
		return true;
	}

	/**
     * @brief Registers a new component type with the ECS in a safe manner.
     * @tparam T The type of the component to be registered.
     * @return True if the component type was successfully registered, false if the component type is already registered.
     */
	template <typename T>
	bool RegisterComponentChecked ()
	{
		return m_ComponentManager . RegisterComponentChecked <T> ();
	}

	/**
     * @brief Adds a new component to an entity in a safe manner.
     * @tparam T The type of the component to be added.
     * @param entityHandle The handle of the entity to which the component will be added.
     * @param component The component to be added.
     * @return True if the component was successfully added, false if the entity handle is invalid or the entity already has the component or the component isn't registered.
     */
	template <typename T>
	bool AddComponentChecked ( EntityHandle entityHandle, const T & component )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		Entity & entity = GetEntity ( entityHandle );
		if ( ! m_ComponentManager . AddComponentChecked ( entity, component ) )
			return false;
		m_SystemManager . OnEntitySignatureChanged ( entity, entity . GetSignature () );
		return true;
	}

	/**
     * @brief Adds a new component to an entity in a safe manner using move semantics.
     * @tparam T The type of the component to be added.
     * @param entityHandle The handle of the entity to which the component will be added.
     * @param component The component to be added.
     * @return True if the component was successfully added, false if the entity handle is invalid or the entity already has the component or the component isn't registered.
     */
	template <typename T>
	bool AddComponentChecked ( EntityHandle entityHandle, T && component )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		Entity & entity = GetEntity ( entityHandle );
		if ( ! m_ComponentManager . AddComponentChecked ( entity, std::forward <T> ( component ) ) )
			return false;
		m_SystemManager . OnEntitySignatureChanged ( entity, entity . GetSignature () );
		return true;
	}


	/**
     * @brief Removes a component of the specified type from an entity in a safe manner.
     * @tparam T The type of the component to be removed.
     * @param entityHandle The handle of the entity from which the component will be removed.
     * @return True if the component was successfully removed, false if the entity handle is invalid or the entity does not have the component.
     */
	template <typename T>
	bool RemoveComponentChecked ( EntityHandle entityHandle )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		Entity & e = m_EntityManager . GetEntity ( entityHandle );
		if ( ! m_ComponentManager . RemoveComponentChecked <T> ( e ) )
			return false;
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
		return true;
	}

	/**
     * @brief Retrieves a safe optional reference to the component of the specified type associated with the given entity.
     * @tparam T The type of the component to retrieve.
     * @param entityHandle The handle of the entity for which to retrieve the component.
     * @return A safe optional reference to the component if it exists, added to entity and is registered, otherwise an empty optional.
     */
	template <typename T>
	std::optional <std::reference_wrapper <T>> GetComponentChecked ( EntityHandle entityHandle )
	{
		auto _entity = GetEntityChecked ( entityHandle );
		if ( ! _entity )
			return std::nullopt;
		Entity & entity = _entity . value () . get ();
		return m_ComponentManager . GetComponentChecked <T> ( entity );
	}

	/**
     * @brief Checks if an entity has a component of the specified type in a safe manner.
     * @tparam T The type of the component to check for.
     * @param entityHandle The handle of the entity to check.
     * @return True if the entity has the component and handle is valid, false otherwise.
     */
	template <typename T>
	bool GetEntityHasComponentChecked ( EntityHandle entityHandle )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) || ! GetIsComponentRegistered <T> () )
			return false;
		return GetEntityHasComponent <T> ( entityHandle );
	}


	/**
     * @brief Registers a new system with the ECS in a safe manner.
     * @tparam T The type of the system to be registered.
     * @tparam ComponentTypes The component types required by the system.
     * @return True if the system was successfully registered, false if the system is already registered.
     */
	template <typename T, typename ... ComponentTypes>
	bool RegisterSystemChecked ()
	{
		return m_SystemManager . RegisterSystemChecked <T> (
				std::forward <Signature> ( GetComponentsSignature <ComponentTypes...> () ) );
	}

	/**
     * @brief Runs a system of the specified type in a safe manner.
     * @tparam T The type of the system to be run.
     * @return True if the system was run successfully, false if the system is not registered.
     */
	template <typename T>
	bool RunSystemChecked ()
	{
		return m_SystemManager . RunSystemChecked <T> ( * this );
	}
	/* End Entity Component System safe interface */
private:

	ComponentManager m_ComponentManager;
	EntityManager m_EntityManager;
	SystemManager m_SystemManager;

};
