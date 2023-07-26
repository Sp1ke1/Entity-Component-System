#pragma once

#include "ObjectManager.h"
#include "Entity.h"
#include <memory>

// Reference: https://austinmorlan.com/posts/entity_component_system/#the-component-array

/**
 * @class ComponentManager
 * @brief Manages components in the Entity-Component-System (ECS) framework.
 *
 * The ComponentManager class is responsible for registering, adding, removing, and accessing components in the ECS framework.
 * It uses an unordered_map to generic ObjectManagers to store and manage component arrays efficiently.
 * The class provides both regular and a safe interface with additional checks for managing components.
 */
class ComponentManager
{

public:

	/* Begin ComponentManager interface */

	/**
     * @brief Registers a new component type.
     * @tparam T The type of the component to be registered.
     */
	template <typename T>
	void RegisterComponent ()
	{
		const auto ComponentType = GetComponentType <T> ();
		m_Components[ ComponentType ] = std::make_shared <ObjectManager <T>> ();
	}

	/**
     * @brief Adds a new component to an entity.
     * @tparam T The type of the component to be added.
     * @param entity The entity to which the component will be added.
     * @param component The component to be added.
     */
	template <typename T>
	void AddComponent ( Entity & entity, const T & component )
	{
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( component );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
	}

	/**
     * @brief Adds a new component to an entity using move semantics.
     * @tparam T The type of the component to be added.
     * @param entity The entity to which the component will be added.
     * @param component The component to be added.
     */
	template <typename T>
	void AddComponent ( Entity & entity, T && component )
	{
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( std::forward <T> ( component ) );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
	}

	/**
     * @brief Removes a component from an entity.
     * @tparam T The type of the component to be removed.
     * @param entity The entity from which the component will be removed.
     */
	template <typename T>
	void RemoveComponent ( Entity & entity )
	{
		const auto ComponentType = GetComponentType <T> ();
		const auto ComponentHandle = entity . GetComponentHandle ( ComponentType );

		auto ComponentArray = GetComponentArray <T> ();

		ComponentArray -> RemoveObject ( ComponentHandle );
		entity . RemoveComponent ( ComponentType );
	}

	/**
     * @brief Retrieves a reference to the component associated with the given entity.
     * @tparam T The type of the component to retrieve.
     * @param entity The entity for which to retrieve the component.
     * @return A reference to the component.
     */
	template <typename T>
	T & GetComponent ( Entity & entity )
	{
		const auto ComponentHandle = entity . GetComponentHandle ( GetComponentType <T> () );
		auto ComponentArray = GetComponentArray <T> ();
		return ComponentArray -> GetObject ( ComponentHandle );
	}

	/**
     * @brief Notifies the component manager that an entity has been removed.
     * @param entity The entity that has been removed.
     */
	void OnEntityRemoved ( Entity & entity )
	{
		const auto & ComponentInfo = entity . GetComponentsInfo ();
		for ( const auto & info : ComponentInfo ) {
			m_Components . at ( info . Type ) -> RemoveObjectChecked ( info . Handle );
		}
	}

	/**
     * @brief Retrieves the ComponentType code for the specified component type.
     * @tparam T The type of the component to get the ComponentType code for.
     * @return The code representing the ComponentType of the specified component.
     */
	template <typename T>
	ComponentType GetComponentType () const
	{
		return typeid ( T ) . hash_code ();
	}


	/**
     * @brief Retrieves a weak pointer to the component array of the specified type.
     * @tparam T The type of the component to retrieve.
     * @return A weak pointer to the component array if registered, otherwise an expired weak pointer.
     */
	template <typename T>
	std::weak_ptr <ObjectManager <T>> GetComponentsByType ()
	{
		return GetIsComponentRegistered <T> () ? GetComponentArray <T> () : nullptr;
	}

	/**
     * @brief Checks if a component of the specified type is registered.
     * @tparam T The type of the component to check for registration.
     * @return True if the component is registered, false otherwise.
     */
	template <typename T>
	bool GetIsComponentRegistered () const
	{
		const auto ComponentType = GetComponentType <T> ();
		return GetIsComponentRegistered ( ComponentType );
	}

	/**
     * @brief Checks if a component with the given ComponentType is registered.
     * @param componentType The ComponentType to check for registration.
     * @return True if a component with the specified ComponentType is registered, false otherwise.
     */
	bool GetIsComponentRegistered ( ComponentType componentType ) const
	{
		return m_Components . count ( componentType ) != 0;
	}
	/* End ComponentManager interface */

	/* Begin ComponentManager safe interface */

	/**
     * @brief Registers a new component type in a safe manner.
     * @tparam T The type of the component to be registered.
     * @return True if the component type was successfully registered, false if the component type is already registered.
     */
	template <typename T>
	bool RegisterComponentChecked ()
	{
		if ( GetIsComponentRegistered <T> () )
			return false;
		RegisterComponent <T>();
		return true;
	}

	/**
     * @brief Adds a new component to an entity in a safe manner.
     * @tparam T The type of the component to be added.
     * @param entity The entity to which the component will be added.
     * @param component The component to be added.
     * @return True if the component was successfully added, false if the component type is not registered or the entity already has the component.
     */
	template <typename T>
	bool AddComponentChecked ( Entity & entity, const T & component )
	{
        if ( ! GetIsComponentRegistered <T> () )
            return false;
        const auto ComponentType = GetComponentType<T> ();
        if ( entity . GetHasComponent( ComponentType ) )
            return false;
        const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( component );
        entity . AddComponent ( { ComponentType, ComponentHandle } );
        return true;
	}

	/**
     * @brief Adds a new component to an entity using move semantics in a safe manner.
     * @tparam T The type of the component to be added.
     * @param entity The entity to which the component will be added.
     * @param component The component to be added.
     * @return True if the component was successfully added, false if the component type is not registered or the entity already has the component.
     */
	template <typename T>
	bool AddComponentChecked ( Entity & entity, T && component )
	{
		if ( ! GetIsComponentRegistered <T> () )
			return false;
        const auto ComponentType = GetComponentType<T> ();
        if ( entity . GetHasComponent( ComponentType ) )
            return false;
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( std::forward <T> ( component ) );
		entity . AddComponent ( { ComponentType, ComponentHandle } );
		return true;
	}

	/**
     * @brief Removes a component from an entity in a safe manner.
     * @tparam T The type of the component to be removed.
     * @param entity The entity from which the component will be removed.
     * @return True if the component was successfully removed, false if the component type is not registered or the entity does not have the component.
     */
	template <typename T>
	bool RemoveComponentChecked ( Entity & entity )
	{
		if ( ! GetIsComponentRegistered <T> () )
			return false;

		const auto ComponentType = GetComponentType <T> ();
		if ( ! entity . GetHasComponent ( ComponentType ) )
			return false;

		const auto ComponentHandle = entity . GetComponentHandle ( ComponentType );
		const auto ComponentArray = GetComponentArray <T> ();
		if ( ! ComponentArray -> RemoveObjectChecked ( ComponentHandle ) )
			return false;
		entity . RemoveComponent ( ComponentType );
		return true;
	}

	/**
     * @brief Retrieves a reference to the component associated with the given entity in a safe manner.
     * @tparam T The type of the component to retrieve.
     * @param entity The entity for which to retrieve the component.
     * @return An optional containing a reference to the component if it exists and is registered, otherwise an empty optional.
     */
	template <typename T>
	std::optional <std::reference_wrapper <T>> GetComponentChecked ( Entity & entity )
	{
		if ( ! GetIsComponentRegistered <T> () )
			return std::nullopt;

		const auto ComponentType = GetComponentType <T> ();
		if ( ! entity . GetHasComponent ( ComponentType ) )
			return std::nullopt;

		const auto handle = entity . GetComponentHandle ( ComponentType );
		const auto ComponentArray = GetComponentArray <T> ();
		return ComponentArray -> GetObjectChecked ( handle );
	}
	/* End ComponentManager safe interface */
private:

	/**
     * @brief Retrieves the component array for the specified component type.
     * @tparam T The type of the component to retrieve the component array for.
     * @return A shared pointer to the component array if registered, otherwise an empty shared pointer.
     */
	template <typename T>
	std::shared_ptr <ObjectManager <T>> GetComponentArray ()
	{
		const auto ComponentType = GetComponentType <T> ();
		return GetComponentArray <T> ( ComponentType );
	}

	/**
     * @brief Retrieves the component array for the specified component type.
     * @tparam T The type of the component to retrieve the component array for.
     * @param componentType The ComponentType of the component to retrieve the component array for.
     * @return A shared pointer to the component array if registered, otherwise an empty shared pointer.
     */
	template <typename T>
	std::shared_ptr <ObjectManager <T>> GetComponentArray ( ComponentType componentType )
	{
		auto Interface = m_Components . at ( componentType );
		return std::static_pointer_cast <ObjectManager <T>> ( Interface );
	}

	std::unordered_map <ComponentType, std::shared_ptr <IObjectManager>> m_Components; /**< The storage for component arrays managed by the ComponentManager. */
};
