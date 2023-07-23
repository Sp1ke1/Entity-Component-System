#pragma once

#include "ObjectManager.h"
#include "Entity.h"

// Reference: https://austinmorlan.com/posts/entity_component_system/#the-component-array

class ComponentManager
{

public:

	/* Begin ComponentManager interface */
	template <typename T>
	void RegisterComponent ()
	{
		const auto ComponentType = GetComponentType <T> ();
		m_Components[ ComponentType ] = std::make_shared <ObjectManager <T>> ();
	}

	template <typename T>
	void AddComponent ( Entity & entity, const T & component )
	{
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( component );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
	}

	template <typename T>
	void AddComponent ( Entity & entity, T && component )
	{
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( std::forward <T> ( component ) );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
	}

	template <typename T>
	void RemoveComponent ( Entity & entity )
	{
		const auto ComponentType = GetComponentType <T> ();
		const auto ComponentHandle = entity . GetComponentHandle ( ComponentType );

		auto ComponentArray = GetComponentArray <T> ();

		ComponentArray -> RemoveObject ( ComponentHandle );
		entity . RemoveComponent ( ComponentType );
	}

	template <typename T>
	T & GetComponent ( Entity & entity )
	{
		const auto ComponentHandle = entity . GetComponentHandle ( GetComponentType <T> () );
		auto ComponentArray = GetComponentArray <T> ();
		return ComponentArray -> GetObject ( ComponentHandle );
	}

	void OnEntityRemoved ( Entity & entity )
	{
		const auto & ComponentInfo = entity . GetComponentsInfo ();
		for ( const auto & info : ComponentInfo ) {
			m_Components . at ( info . Type ) -> RemoveObjectChecked ( info . Handle );
		}
	}

	template <typename T>
	ComponentType GetComponentType () const
	{
		return typeid ( T ) . hash_code ();
	}

	template <typename T>
	std::weak_ptr <ObjectManager <T>> GetComponentsByType ()
	{
		return GetIsComponentRegistered <T> () ? GetComponentArray <T> () : nullptr;
	}

	template <typename T>
	bool GetIsComponentRegistered () const
	{
		const auto ComponentType = GetComponentType <T> ();
		return GetIsComponentRegistered ( ComponentType );
	}

	bool GetIsComponentRegistered ( ComponentType componentType ) const
	{
		return m_Components . count ( componentType ) != 0;
	}
	/* End ComponentManager interface */

	/* Begin ComponentManager safe interface */
	template <typename T>
	bool RegisterComponentChecked ()
	{
		const auto ComponentType = GetComponentType <T> ();
		if ( m_Components . count ( ComponentType ) == 0 )
			return false;
		m_Components[ ComponentType ] = std::make_shared <ObjectManager <T>> ();
		return true;
	}

	template <typename T>
	bool AddComponentChecked ( Entity & entity, const T & component )
	{
		if ( ! GetIsComponentRegistered <T> () )
			return false;
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( component );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
		return true;
	}

	template <typename T>
	bool AddComponentChecked ( Entity & entity, T && component )
	{
		if ( ! GetIsComponentRegistered <T> () )
			return false;
		const auto ComponentHandle = GetComponentArray <T> () -> AddObject ( std::forward <T> ( component ) );
		entity . AddComponent ( { GetComponentType <T> (), ComponentHandle } );
		return true;
	}

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

	template <typename T>
	std::shared_ptr <ObjectManager <T>> GetComponentArray ()
	{
		const auto ComponentType = GetComponentType <T> ();
		return GetComponentArray <T> ( ComponentType );
	}

	template <typename T>
	std::shared_ptr <ObjectManager <T>> GetComponentArray ( ComponentType componentType )
	{
		auto Interface = m_Components . at ( componentType );
		return std::static_pointer_cast <ObjectManager <T>> ( Interface );
	}

	std::unordered_map <ComponentType, std::shared_ptr <IObjectManager>> m_Components;
};
