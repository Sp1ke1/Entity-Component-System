#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECS
{

public:

	// Begin Entity Component System interface

	EntityHandle CreateEntity ()
	{
		return m_EntityManager . CreateEntity ();
	}

	void RemoveEntity ( EntityHandle entityHandle )
	{
		Entity & e = m_EntityManager . GetEntity ( entityHandle );
		m_ComponentManager . OnEntityRemoved ( e );
		m_SystemManager . OnEntityRemoved ( e );
		m_EntityManager . RemoveEntity ( entityHandle );
	}

	Entity & GetEntity ( EntityHandle entityHandle )
	{
		return m_EntityManager . GetEntity ( entityHandle );
	}

	template <typename T>
	bool GetEntityHasComponent ( EntityHandle entityHandle )
	{
		const auto ComponentType = GetComponentType <T> ();
		Entity & e = GetEntity ( entityHandle );
		return e. GetHasComponent ( ComponentType );
	}

	bool GetIsValidEntityHandle ( EntityHandle entityHandle ) const
	{
		return m_EntityManager . GetIsValidEntityHandle ( entityHandle );
	}

	template <typename T>
	void RegisterComponent ()
	{
		return m_ComponentManager . RegisterComponent <T> ();
	}

	template <typename T>
	void AddComponent ( EntityHandle entityHandle, T && component )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . AddComponent ( e, std::forward <T> ( component ) );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	template <typename T>
	void AddComponent ( EntityHandle entityHandle, const T & component )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . AddComponent ( e, component );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	template <typename T>
	void RemoveComponent ( EntityHandle entityHandle )
	{
		Entity & e = GetEntity ( entityHandle );
		m_ComponentManager . RemoveComponent <T> ( e );
		m_SystemManager . OnEntitySignatureChanged ( e, e . GetSignature () );
	}

	template <typename T>
	T & GetComponent ( EntityHandle entity )
	{
		Entity & e = GetEntity ( entity );
		return m_ComponentManager . GetComponent <T> ( e );
	}

	template <typename T>
	ComponentType GetComponentType ()
	{
		return m_ComponentManager . GetComponentType <T> ();
	}

	template <typename T>
	std::weak_ptr <ObjectManager <T>> GetComponentsByType ()
	{
		return m_ComponentManager . GetComponentsByType <T> ();
	}

	template <typename T>
	bool GetIsComponentRegistered () const
	{
		return m_ComponentManager . GetIsComponentRegistered <T> ();
	}

	template <typename T, typename ... ComponentTypes>
	void RegisterSystem ()
	{
		return m_SystemManager . RegisterSystem <T> ( std::move ( GetComponentsSignature <ComponentTypes ...> () ) );
	}

	template <typename T>
	void RunSystem ()
	{
		m_SystemManager . RunSystem <T> ( * this );
	}

	template <typename T>
	std::weak_ptr <T> GetSystem ()
	{
		return m_SystemManager . GetSystem <T> ();
	}

	template <typename T>
	SystemType GetSystemType () const
	{
		return m_SystemManager . GetSystemType <T> ();
	}

	template <typename T>
	bool GetIsSystemRegistered () const
	{
		return m_SystemManager . GetIsSystemRegistered <T> ();
	}

	template <typename ... ComponentTypes>
	Signature GetComponentsSignature () const
	{
		return { m_ComponentManager . GetComponentType <ComponentTypes> () ... };
	}
	/* End Entity Component System interface */


	/* Begin Entity Component System safe interface */

	std::optional <std::reference_wrapper <Entity>> GetEntityChecked ( EntityHandle entityHandle )
	{
		return m_EntityManager . GetEntityChecked ( entityHandle );
	}

	bool RemoveEntityChecked ( EntityHandle entityHandle )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		RemoveEntity ( entityHandle );
		return true;
	}

	template <typename T>
	bool RegisterComponentChecked ()
	{
		return m_ComponentManager . RegisterComponentChecked <T> ();
	}

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

	template <typename T>
	std::optional <std::reference_wrapper <T>> GetComponentChecked ( EntityHandle entityHandle )
	{
		auto _entity = GetEntityChecked ( entityHandle );
		if ( ! _entity )
			return std::nullopt;
		Entity & entity = _entity . value () . get ();
		return m_ComponentManager . GetComponentChecked <T> ( entity );
	}

	template <typename T>
	bool GetEntityHasComponentChecked ( EntityHandle entityHandle )
	{
		if ( ! GetIsValidEntityHandle ( entityHandle ) )
			return false;
		return GetEntityHasComponent <T> ( entityHandle );
	}


	template <typename T, typename ... ComponentTypes>
	bool RegisterSystemChecked ()
	{
		return m_SystemManager . RegisterSystemChecked <T> (
				std::forward <Signature> ( GetComponentsSignature <ComponentTypes...> () ) );
	}

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
