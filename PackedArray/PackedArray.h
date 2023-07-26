#pragma once

#include <vector>
#include <unordered_map>
#include <optional>
#include <set>

using PackedArrayHandle = std::size_t;

/**
 * @class PackedArray
 * @brief Dynamic array implementation optimized for storage and cache locality.
 * @tparam T The type of elements stored in the PackedArray.
 *
 * The PackedArray class is an optimized dynamic array implementation for storage and cache locality.
 * It allows efficient insertion, removal, and access to elements using handles.
 * The class provides both a regular interface and a safe interface with additional checks.
 */
template <typename T>
class PackedArray
{
public:

	// --- Begin PackedArray interface

    /**
    * @brief Emplace a new object at the back of the PackedArray.
    * @tparam Args Variadic template parameter pack for object construction arguments.
    * @param Arguments The arguments forwarded to the constructor of the object.
    * @return The handle of the newly emplaced object.
    */
    template <typename ... Args>
    PackedArrayHandle EmplaceBack ( Args && ... Arguments )
    {
        const auto Handle = AllocateHandleForIndex ( m_Data . size () );
        m_Data . emplace_back ( std::forward <Args> ( Arguments ) ... );
        return Handle;
    }

    /**
     * @brief Add a new object to the PackedArray.
     * @param object The object to be added to the PackedArray.
     * @return The handle of the newly added object.
     */
	PackedArrayHandle Add ( T && object )
	{
		const auto Handle = AllocateHandleForIndex ( m_Data . size () );
		m_Data . push_back ( std::forward <T> ( object ) );
		return Handle;
	}

    /**
     * @brief Add a new object to the PackedArray.
     * @param object The object to be added to the PackedArray.
     * @return The handle of the newly added object.
     */
	PackedArrayHandle Add ( const T & object )
	{
		const auto Handle = AllocateHandleForIndex ( m_Data . size () );
		m_Data . push_back ( object );
		return Handle;
	}

    /**
     * @brief Remove an object from the PackedArray by handle.
     * @param handle The handle of the object to be removed.
     *
     * This method removes an object from the PackedArray based on its handle.
     * The object is removed and its handle is released for reuse.
     */
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

    /**
     * @brief Remove an object from the PackedArray by index.
     * @param index The index of the object to be removed..
     */
	void RemoveByIndex ( std::size_t index )
	{
		const auto Handle = m_IndexToHandle . at ( index );
		Remove ( Handle );
	}

    /**
     * @brief Get a reference to the object associated with a given handle.
     * @param handle The handle of the object to retrieve.
     * @return A reference to the object associated with the specified handle.
     */
    T & operator [] ( PackedArrayHandle handle )
    {
        return Get ( handle );
    }

    /**
     * @brief Get a reference to the object associated with a given handle.
     * @param handle The handle of the object to retrieve.
     * @return A reference to the object associated with the specified handle.
     */
    T & Get ( PackedArrayHandle handle )
    {
        return m_Data[ m_HandleToIndex . at ( handle ) ];
    }

    /**
     * @brief Get a reference to the object associated with a given index.
     * @param index The index of the object to retrieve.
     * @return A reference to the object associated with the specified index.
     */
    T & GetByIndex ( std::size_t index )
    {
        return m_Data [ index ];
    }

    /**
     * @brief Check if a given handle is valid in the PackedArray.
     * @param handle The handle to be checked.
     * @return True if the handle is valid, false otherwise.
     */
	bool IsValidHandle ( PackedArrayHandle handle ) const
	{
		return m_HandleToIndex . count ( handle ) != 0;
	}

    /**
     * @brief Check if a given index is valid in the PackedArray.
     * @param index The index to be checked.
     * @return True if the index is valid, false otherwise.
     */
	bool IsValidIndex ( std::size_t index ) const
	{
		return index < m_Data.size();
	}

    /**
     * @brief Get the handle associated with an index in the PackedArray.
     * @param index The index of the object.
     * @return The handle associated with the specified index.

     */
	PackedArrayHandle GetHandleFromIndex (std::size_t index ) const
	{
		return m_IndexToHandle . at ( index );
	}

    /**
     * @brief Get the size of the PackedArray.
     * @return The number of elements currently stored in the PackedArray.
     */
	std::size_t Size() const
	{
		return m_Data.size();
	}

    /**
     * @brief Clear the PackedArray, removing all elements and invalidating all handles
     */
	void Clear ()
	{
		m_Data . clear ();
		m_HandleToIndex . clear ();
		m_IndexToHandle . clear ();
        m_FreeHandles . clear();
	}

    /**
     * @brief Get an iterator to the beginning of the PackedArray.
     * @return An iterator to the beginning of the PackedArray.
     */
	typename std::vector <T>::iterator begin ()
	{
		return m_Data . begin ();
	}

    /**
     * @brief Get an iterator to the end of the PackedArray.
     * @return An iterator to the end of the PackedArray.
     */
	typename std::vector <T>::iterator end ()
	{
		return m_Data . end ();
	}

    /**
     * @brief Get a const iterator to the beginning of the PackedArray.
     * @return A const iterator to the beginning of the PackedArray.
     */
	typename std::vector <T>::const_iterator begin () const
	{
		return m_Data . begin ();
	}

    /**
     * @brief Get a const iterator to the end of the PackedArray.
     * @return A const iterator to the end of the PackedArray.
     */
	typename std::vector <T>::const_iterator end () const
	{
		return m_Data . end ();
	}

    /**
     * @brief Get the next available handle in the PackedArray.
     * @return The next available handle for insertion.
     */
	PackedArrayHandle GetNextHandle () const
	{
		return m_FreeHandles . empty () ? m_Data . size () : * m_FreeHandles . begin ();
	}

	// --- End PackedArray interface

    // --- Begin PackedArray safe interface

    /**
     * @brief Remove an object from the PackedArray by handle, with error-checking.
     * @param handle The handle of the object to be removed.
     * @return True if the removal was successful, false otherwise (if the handle is invalid).
     */
    bool RemoveChecked ( PackedArrayHandle handle )
    {
        if ( ! IsValidHandle ( handle ) )
            return false;
        Remove ( handle );
        return true;
    }

    /**
     * @brief Remove an object from the PackedArray by index, with error-checking.
     * @param index The index of the object to be removed.
     * @return True if the removal was successful, false otherwise (if the index is out of bounds).
     */
    bool RemoveByIndexChecked ( std::size_t index )
    {
        if ( ! IsValidIndex ( index ) )
            return false;
        RemoveByIndex ( index );
        return true;
    }

    /**
     * @brief Safely get a reference to the object associated with a given handle.
     * @param handle The handle of the object to retrieve.
     * @return An optional reference to the object associated with the specified handle, or std::nullopt if the handle is invalid.
     */
    std::optional <std::reference_wrapper <T>> GetChecked ( PackedArrayHandle handle )
    {
        if ( ! IsValidHandle ( handle ) )
            return std::nullopt;

        return Get ( handle );
    }

    /**
    * @brief Safely get a reference to the object associated with a given index.
    * @param index The index of the object to retrieve.
    * @return An optional reference to the object associated with the specified index, or std::nullopt if the index is invalid.
    */
    std::optional <std::reference_wrapper<T>> GetByIndexChecked ( std::size_t index )
    {
        if ( ! IsValidIndex ( index ) )
            return std::nullopt;
        return GetByIndex ( index );
    }

    /**
     * @brief Safely get the handle associated with an index in the PackedArray, with error-checking.
     * @param index The index of the object.
     * @return The handle associated with the specified index, or std::nullopt if the index is invalid.
     */
    std::optional<PackedArrayHandle> GetHandleFromIndexChecked (std::size_t index ) const
    {
        if ( ! IsValidIndex ( index ) )
            return std::nullopt;
        return GetHandleFromIndex(index);
    }
    // --- End PackedArray safe interface

private:

    /**
     * @brief Allocate a handle for a given index in the PackedArray.
     * @param index The index for which a handle needs to be allocated.
     * @return The allocated handle for the specified index.
     */
	PackedArrayHandle AllocateHandleForIndex ( std::size_t index )
	{
		PackedArrayHandle Handle;
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

	std::vector <T> m_Data; /**< The container to hold the elements of the PackedArray. */

	std::unordered_map <PackedArrayHandle, std::size_t> m_HandleToIndex; /**< Mapping of handles to their corresponding indices. */
	std::unordered_map <std::size_t, PackedArrayHandle> m_IndexToHandle; /**< Mapping of indices to their corresponding handles. */
	std::set <PackedArrayHandle> m_FreeHandles; /**< Set of free handles that can be reused when elements are removed. */
};

