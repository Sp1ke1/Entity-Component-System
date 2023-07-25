#pragma once

#include "ObjectManager.h"
#include "System.h"

class SystemManager
{


public:

	/* Begin SystemManager interface */

	template <typename T>
	void RegisterSystem ( Signature && SystemSignature )
	{
		const auto SystemType = GetSystemType <T> ();
		m_Systems[ SystemType ] = std::move ( std::make_shared <T> () );
		m_Systems . at ( SystemType ) -> SetSignature ( std::forward <Signature> ( SystemSignature ) );
	}

	template <typename T>
	void RunSystem ( ECS & e )
	{
		std::shared_ptr <T> System = GetSystem <T> () . lock ();
		System -> Run ( e );
	}

	template <typename T>
	std::weak_ptr <T> GetSystem ()
	{
		auto System = GetSystem ( GetSystemType <T> () );
		return System . lock () ? std::static_pointer_cast <T> ( System . lock () ) : nullptr;
	}

	std::weak_ptr <System> GetSystem ( SystemType systemType )
	{
		return m_Systems . count ( systemType ) == 0 ? nullptr : m_Systems . at ( systemType );
	}

	template <typename T>
	SystemType GetSystemType () const
	{
		return typeid ( T ) . hash_code ();
	}

	template <typename T>
	bool GetIsSystemRegistered () const
	{
		const auto SystemType = GetSystemType <T> ();
		return GetIsSystemRegistered ( SystemType );
	}

	bool GetIsSystemRegistered ( SystemType systemType ) const
	{
		return m_Systems . count ( systemType ) != 0;
	}

	void OnEntitySignatureChanged ( Entity & e, const Signature & signature )
	{
		for ( auto & System : m_Systems ) {
			System . second -> OnEntitySignatureChanged ( e, signature );
		}
	}

	void OnEntityRemoved ( Entity & e )
	{
		for ( auto & System : m_Systems ) {
			System . second -> RemoveEntityChecked ( e );
		}
	}
	/* End SystemManager interface */

	/* Begin SystemManager safe interface */
	template <typename T>
	bool RegisterSystemChecked ( Signature && SystemSignature )
	{
		if ( GetIsSystemRegistered <T> () )
			return false;
		RegisterSystem <T> ( std::forward <Signature> ( SystemSignature ) );
		return true;
	}

	template <typename T>
	bool RunSystemChecked ( ECS & e )
	{
		std::shared_ptr <T> System = GetSystem <T> () . lock ();
		if ( ! System )
			return false;
		System -> Run ( e );
		return true;
	}
	/* End SystemManager safe interface */
private:

	std::unordered_map <SystemType, std::shared_ptr <System>> m_Systems;
};