#pragma once

#include "PackedArray.h"


class IObjectManager
{
public:
	virtual ~IObjectManager () = default;

	virtual bool RemoveObjectChecked ( ObjectHandle handle ) = 0;

	virtual void RemoveObject ( ObjectHandle handle ) = 0;

	virtual void Clear () = 0;
};

template <typename ObjectType>
class ObjectManager : public IObjectManager
{

public:

	template <typename ... Args>
	ObjectHandle CreateObject ( Args && ... Arguments )
	{
		return m_Objects . EmplaceBack ( std::forward <Args> ( Arguments ) ... );
	}

	ObjectHandle AddObject ( const ObjectType & object )
	{
		return m_Objects . Add ( object );
	}

	ObjectHandle AddObject ( ObjectType && object )
	{
		return m_Objects . Add ( std::forward <ObjectType> ( object ) );
	}

	void RemoveObject ( ObjectHandle handle ) override
	{
		return m_Objects . Remove ( handle );
	}

	void RemoveObjectByIndex ( std::size_t index )
	{
		m_Objects . RemoveByIndex ( index );
	}

	ObjectType & GetObject ( ObjectHandle handle )
	{
		return m_Objects . Get ( handle );
	}

	ObjectType & GetObjectByIndex ( std::size_t index )
	{
		return m_Objects . GetByIndex ( index );
	}

	ObjectHandle HandleFromIndex ( std::size_t index ) const
	{
		return m_Objects . HandleFromIndex ( index );
	}

	bool GetIsValidHandle ( ObjectHandle handle ) const
	{
		return m_Objects . IsValidHandle ( handle );
	}

	void Clear () override
	{
		m_Objects . Clear ();
	}

	std::size_t Size () const
	{
		return m_Objects . Size ();
	}

	ObjectHandle GetNextHandle () const
	{
		return m_Objects . GetNextHandle ();
	}

	typename std::vector <ObjectType>::iterator begin ()
	{
		return m_Objects . begin ();
	}

	typename std::vector <ObjectType>::iterator end ()
	{
		return m_Objects . end ();
	}

	typename std::vector <ObjectType>::const_iterator begin () const
	{
		return m_Objects . begin ();
	}

	typename std::vector <ObjectType>::const_iterator end () const
	{
		return m_Objects . end ();
	}
	/* End ObjectManager interface */

	/* Begin ObjectManager safe interface */
	bool RemoveObjectChecked ( ObjectHandle handle ) override
	{
		return m_Objects . RemoveChecked ( handle );
	}

	bool RemoveObjectByIndexChecked ( std::size_t index )
	{
		return m_Objects . RemoveByIndexChecked ( index );
	}

	std::optional <std::reference_wrapper <ObjectType>> GetObjectChecked ( ObjectHandle handle )
	{
		return m_Objects . GetChecked ( handle );
	}


	std::optional <std::reference_wrapper <ObjectType>> GetObjectByIndexChecked ( std::size_t index )
	{
		return m_Objects . GetByIndexChecked ( index );
	}

	std::optional <ObjectHandle> HandleFromIndexChecked ( std::size_t index ) const
	{
		return m_Objects . HandleFromIndexChecked ( index );
	}
	/* End ObjectManager safe interface */

protected:
	PackedArray <ObjectType> m_Objects;
};




