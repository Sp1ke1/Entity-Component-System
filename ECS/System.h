#pragma once

#include "ObjectManager.h"
#include "Entity.h"

class ECS;

class System
{

public:

	System ( Signature && SystemSignature )
			: m_Signature ( std::move ( SystemSignature ) )
	{};

	System ( const Signature & SystemSignature )
			: m_Signature ( SystemSignature )
	{};

	System () = default;

	virtual ~System () = default;

	void AddEntity ( Entity & e )
	{
		const auto Handle = m_Entities . AddObject ( e . GetHandle () );
		m_EntitiesHandles[ e . GetHandle () ] = Handle;
	}

	bool AddEntityChecked ( Entity & e )
	{
		if ( m_EntitiesHandles . count ( e . GetHandle () ) != 0 )
			return false;
		AddEntity ( e );
		return true;
	}

	bool RemoveEntityChecked ( Entity & e )
	{
		if ( m_EntitiesHandles . count ( e . GetHandle () ) == 0 )
			return false;
		RemoveEntity ( e );
		return true;
	}

	void RemoveEntity ( Entity & e )
	{
		m_Entities . RemoveObject ( m_EntitiesHandles . at ( e . GetHandle () ) );
		m_EntitiesHandles . erase ( e . GetHandle () );
	}

	void SetSignature ( const Signature & signature )
	{
		m_Signature = signature;
	}

	void SetSignature ( Signature && signature )
	{
		m_Signature = std::move ( signature );
	}

	void OnEntitySignatureChanged ( Entity & e, const Signature & signature )
	{
		const bool Match = std::includes ( signature . begin (), signature . end (), m_Signature . begin (),
										   m_Signature . end () );
		if ( Match ) {
			AddEntityChecked ( e );
		} else {
			RemoveEntityChecked ( e );
		}
	}

	virtual void Run ( ECS & e ) = 0;

private:
	Signature m_Signature;
	std::unordered_map <EntityHandle, ObjectHandle> m_EntitiesHandles;
protected:
	ObjectManager <EntityHandle> m_Entities;
};