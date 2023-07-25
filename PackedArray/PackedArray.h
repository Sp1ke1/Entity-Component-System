#pragma once

#include "ECSInclude.h" // TODO: solve includes problem. Refactor all includes

using PackedArrayHandle = std::size_t;

enum HandleInjectionMethod
{
	NoInjection,
	Constructor,
};


template <typename T>
class PackedArray
{
public:

	// --- Begin PackedArray interface
	template <typename ... Args>
	PackedArrayHandle EmplaceBack ( Args && ... Arguments )
	{
		const auto Handle = AllocateHandleForIndex ( m_Data . size () );
		m_Data . push_back ( T ( std::forward <Args> ( Arguments ) ... ) );
		return Handle;
	}

	PackedArrayHandle Add ( T && object )
	{
		const auto Handle = AllocateHandleForIndex ( m_Data . size () );
		m_Data . push_back ( std::forward <T> ( object ) );
		return Handle;
	}

	PackedArrayHandle Add ( const T & object )
	{
		const auto Handle = AllocateHandleForIndex ( m_Data . size () );
		m_Data . push_back ( object );
		return Handle;
	}

	void Remove ( PackedArrayHandle handle )
	{
		// Fetch all data of swapped elements
		const auto ElementToRemoveIndex = m_HandleToIndex[ handle ];
		const auto ElementToRemoveHandle = m_IndexToHandle[ ElementToRemoveIndex ];
		const auto LastElementIndex = m_Data . size () - 1;
		const auto LastElementHandle = m_IndexToHandle[ LastElementIndex ];

		// Swap and remove
		m_Data[ ElementToRemoveIndex ] = std::move ( m_Data[ LastElementIndex ] );
		m_Data . pop_back ();

		// Fix-up index to handle
		m_IndexToHandle[ ElementToRemoveIndex ] = LastElementHandle;
		m_IndexToHandle . erase ( LastElementIndex );

		// Fix-up handle to index
		m_HandleToIndex[ LastElementHandle ] = ElementToRemoveIndex;
		m_HandleToIndex . erase ( ElementToRemoveHandle );

		m_FreeHandles . insert ( ElementToRemoveHandle );
	}

	void RemoveByIndex ( std::size_t index )
	{
		const auto Handle = m_IndexToHandle . at ( index );
		Remove ( Handle );
	}

	bool IsValidHandle ( PackedArrayHandle handle ) const
	{
		return m_HandleToIndex . count ( handle ) != 0;
	}

	bool IsValidIndex ( std::size_t index ) const
	{
		return index < m_Data.size();
	}

	PackedArrayHandle HandleFromIndex ( std::size_t index ) const
	{
		return m_IndexToHandle . at ( index );
	}

	std::optional<PackedArrayHandle> HandleFromIndexChecked ( std::size_t index ) const
	{
		if ( ! IsValidIndex ( index ) )
			return std::nullopt;
		return HandleFromIndex ( index );
	}

	std::size_t Size() const
	{
		return m_Data.size();
	}

	void Clear ()
	{
		m_Data . clear ();
		m_HandleToIndex . clear ();
		m_IndexToHandle . clear ();
	}

	T & operator [] ( PackedArrayHandle handle )
	{
		return Get ( handle );
	}

	T & Get ( PackedArrayHandle handle )
	{
		return m_Data[ m_HandleToIndex . at ( handle ) ];
	}

	T & GetByIndex ( std::size_t index )
	{
		return m_Data [ index ];
	}

	typename std::vector <T>::iterator begin ()
	{
		return m_Data . begin ();
	}

	typename std::vector <T>::iterator end ()
	{
		return m_Data . end ();
	}

	typename std::vector <T>::const_iterator begin () const
	{
		return m_Data . begin ();
	}

	typename std::vector <T>::const_iterator end () const
	{
		return m_Data . end ();
	}

	PackedArrayHandle GetNextHandle () const
	{
		return m_FreeHandles . empty () ? m_Data . size () : * m_FreeHandles . begin ();
	}

	// --- End PackedArray interface

	// --- Begin PackedArray safe interface
	std::optional <std::reference_wrapper <T>> GetChecked ( PackedArrayHandle handle )
	{
		if ( ! IsValidHandle ( handle ) )
			return std::nullopt;

		return Get ( handle );
	}

	std::optional <std::reference_wrapper<T>> GetByIndexChecked ( std::size_t index )
	{
		if ( ! IsValidIndex ( index ) )
			return std::nullopt;
		return GetByIndex ( index );
	}

	bool RemoveChecked ( PackedArrayHandle handle )
	{
		if ( ! IsValidHandle ( handle ) )
			return false;
		Remove ( handle );
		return true;
	}

	bool RemoveByIndexChecked ( std::size_t index )
	{
		if ( ! IsValidIndex ( index ) )
			return false;
		RemoveByIndex ( index );
		return true;
	}
	// --- End PackedArray safe interface

private:

	PackedArrayHandle AllocateHandleForIndex ( std::size_t index )
	{
		PackedArrayHandle Handle = 0;
		if ( m_FreeHandles . empty () ) {
			index <= m_Data . size () ? Handle = m_Data . size () : Handle = index;
		} else {
			Handle = * m_FreeHandles . begin ();
			m_FreeHandles . erase ( m_FreeHandles . begin () );
		}

		m_HandleToIndex[ Handle ] = index;
		m_IndexToHandle[ index ] = Handle;
		return Handle;
	}

	std::vector <T> m_Data;

	std::unordered_map <PackedArrayHandle, std::size_t> m_HandleToIndex;
	std::unordered_map <std::size_t, PackedArrayHandle> m_IndexToHandle;
	std::set <PackedArrayHandle> m_FreeHandles;
};
