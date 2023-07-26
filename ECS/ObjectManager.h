#pragma once

#include "PackedArray.h"
#include "Types.h"


/**
 * @class IObjectManager
 * @brief Base object manager interface to store it in a polymorphic manner
 */
class IObjectManager
{
public:
	virtual ~IObjectManager () = default;

	virtual bool RemoveObjectChecked ( ObjectHandle handle ) = 0;

	virtual void RemoveObject ( ObjectHandle handle ) = 0;

	virtual void Clear () = 0;
};

/**
 * @class ObjectManager
 * @brief Manages objects of a specific ObjectType.
 *
 * The ObjectManager class is responsible for creating, removing, and accessing objects of a specific ObjectType.
 * It's a wrapper around PackedArray class with additional IObjectManager inheritance for polymorphism
 *
 * @tparam ObjectType The type of objects managed by this manager.
 */
template <typename ObjectType>
class ObjectManager : public IObjectManager
{

public:

	/**
     * @brief Creates a new object with the specified arguments and adds it to the manager.
     * @tparam Args The types of arguments to construct the object.
     * @param Arguments The arguments to construct the object.
     * @return The handle of the newly created object.
     */
	template <typename ... Args>
	ObjectHandle CreateObject ( Args && ... Arguments )
	{
		return m_Objects . EmplaceBack ( std::forward <Args> ( Arguments ) ... );
	}

	/**
     * @brief Adds an existing object to the manager with its copy.
     * @param object The object to be added.
     * @return The handle of the added object.
     */
	ObjectHandle AddObject ( const ObjectType & object )
	{
		return m_Objects . Add ( object );
	}

	/**
     * @brief Adds an existing object to the manager with its move.
     * @param object The object to be added.
     * @return The handle of the added object.
     */
	ObjectHandle AddObject ( ObjectType && object )
	{
		return m_Objects . Add ( std::forward <ObjectType> ( object ) );
	}

	/**
	* @brief Removes an object from the manager.
	* @param handle The handle of the object to be removed.
	*/
	void RemoveObject ( ObjectHandle handle ) override
	{
		return m_Objects . Remove ( handle );
	}

	/**
     * @brief Removes an object from the manager by its index.
     * @param index The index of the object to be removed.
     */
	void RemoveObjectByIndex ( std::size_t index )
	{
		m_Objects . RemoveByIndex ( index );
	}

	/**
     * @brief Retrieves a reference to the object associated with the given handle.
     * @param handle The handle of the object to retrieve.
     * @return A reference to the object.
     */
	ObjectType & GetObject ( ObjectHandle handle )
	{
		return m_Objects . Get ( handle );
	}

	/**
     * @brief Retrieves a reference to the object associated with the given index.
     * @param index The index of the object to retrieve.
     * @return A reference to the object.
     */
	ObjectType & GetObjectByIndex ( std::size_t index )
	{
		return m_Objects . GetByIndex ( index );
	}

	/**
     * @brief Retrieves the handle of the object associated with the given index.
     * @param index The index of the object to retrieve its handle.
     * @return The handle of the object.
     */
	ObjectHandle GetHandleFromIndex (std::size_t index ) const
	{
		return m_Objects.GetHandleFromIndex(index);
	}

	/**
     * @brief Checks if the given object handle is valid.
     * @param handle The handle of the object to check.
     * @return True if the handle is valid, false otherwise.
     */
	bool GetIsValidHandle ( ObjectHandle handle ) const
	{
		return m_Objects . IsValidHandle ( handle );
	}

	/**
     * @brief Clears all objects from the manager.
     */
	void Clear () override
	{
		m_Objects . Clear ();
	}

	/**
     * @brief Retrieves the number of objects in the manager.
     * @return The number of objects in the manager.
     */
	std::size_t Size () const
	{
		return m_Objects . Size ();
	}

	/**
     * @brief Retrieves the next available handle for creating a new object.
     * @return The next available handle.
     */
	ObjectHandle GetNextHandle () const
	{
		return m_Objects . GetNextHandle ();
	}

	/**
     * @brief Returns an iterator to the beginning of the objects in the manager.
     * @return An iterator pointing to the first object.
     */
	typename std::vector <ObjectType>::iterator begin ()
	{
		return m_Objects . begin ();
	}

	/**
     * @brief Returns an iterator to the end of the objects in the manager.
     * @return An iterator pointing to the position after the last object.
     */
	typename std::vector <ObjectType>::iterator end ()
	{
		return m_Objects . end ();
	}

	/**
     * @brief Returns a const iterator to the beginning of the objects in the manager.
     * @return A const iterator pointing to the first object.
     */
	typename std::vector <ObjectType>::const_iterator begin () const
	{
		return m_Objects . begin ();
	}

	/**
     * @brief Returns a const iterator to the end of the objects in the manager.
     * @return A const iterator pointing to the position after the last object.
     */
	typename std::vector <ObjectType>::const_iterator end () const
	{
		return m_Objects . end ();
	}
	/* End ObjectManager interface */

	/* Begin ObjectManager safe interface */

	/**
     * @brief Removes an object from the manager with additional safety checks.
     * @param handle The handle of the object to be removed.
     * @return True if the object was removed successfully, false otherwise (e.g., handle not found).
     */
	bool RemoveObjectChecked ( ObjectHandle handle ) override
	{
		return m_Objects . RemoveChecked ( handle );
	}

	/**
     * @brief Removes an object from the manager by its index with additional safety checks.
     * @param index The index of the object to be removed.
     * @return True if the object was removed successfully, false otherwise (e.g., index out of bounds).
     */
	bool RemoveObjectByIndexChecked ( std::size_t index )
	{
		return m_Objects . RemoveByIndexChecked ( index );
	}

	/**
     * @brief Retrieves a reference to the object associated with the given handle with additional safety checks.
     * @param handle The handle of the object to retrieve.
     * @return An optional containing a reference to the object if it exists, or an empty optional if not found.
     */
	std::optional <std::reference_wrapper <ObjectType>> GetObjectChecked ( ObjectHandle handle )
	{
		return m_Objects . GetChecked ( handle );
	}

	/**
     * @brief Retrieves a reference to the object associated with the given index with additional safety checks.
     * @param index The index of the object to retrieve.
     * @return An optional containing a reference to the object if it exists, or an empty optional if not found.
     */
	std::optional <std::reference_wrapper <ObjectType>> GetObjectByIndexChecked ( std::size_t index )
	{
		return m_Objects . GetByIndexChecked ( index );
	}

	/**
     * @brief Retrieves the handle of the object associated with the given index with additional safety checks.
     * @param index The index of the object to retrieve its handle.
     * @return An optional containing the handle of the object if it exists, or an empty optional if not found.
     */
	std::optional <ObjectHandle> GetHandleFromIndexChecked (std::size_t index ) const
	{
		return m_Objects.GetHandleFromIndexChecked(index);
	}
	/* End ObjectManager safe interface */

protected:
	PackedArray <ObjectType> m_Objects; /**< The storage for objects managed by the ObjectManager. */
};




