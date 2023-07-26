#pragma once

#include "ObjectManager.h"
#include "System.h"

/**
 * @class SystemManager
 * @brief Manages systems in the Entity-Component-System (ECS) framework.
 *
 * The SystemManager class is responsible for registering, running, and accessing systems in the ECS framework.
 * It stores systems in an unordered_map and provides methods for handling their interactions with entities.
 * The class provides both a fast regular interface and a safe interface with additional checks.
 */
class SystemManager
{


public:

	/* Begin SystemManager interface */

	/**
     * @brief Registers a new system with the specified signature.
     * @tparam T The type of the system to be registered.
     * @param SystemSignature The signature of the system to be registered.
     */
	template <typename T>
	void RegisterSystem ( Signature && SystemSignature )
	{
		const auto SystemType = GetSystemType <T> ();
		m_Systems[ SystemType ] = std::move ( std::make_shared <T> () );
		m_Systems . at ( SystemType ) -> SetSignature ( std::forward <Signature> ( SystemSignature ) );
	}

	/**
	* @brief Runs the specified system for the provided ECS instance.
	* @tparam T The type of the system to be run.
	* @param ecs The ECS instance to run the system on.
	*/
	template <typename T>
	void RunSystem ( ECS & ecs )
	{
		std::shared_ptr <T> System = GetSystem <T> () . lock ();
		System -> Run ( ecs );
	}

	/**
     * @brief Retrieves a weak pointer to the specified system type.
     * @tparam T The type of the system to retrieve.
     * @return A weak pointer to the system if found, otherwise an expired weak pointer.
     */
	template <typename T>
	std::weak_ptr <T> GetSystem ()
	{
		auto System = GetSystem ( GetSystemType <T> () );
		return System . lock () ? std::static_pointer_cast <T> ( System . lock () ) : nullptr;
	}

	/**
     * @brief Retrieves a weak pointer to the system with the given SystemType.
     * @param systemType The SystemType of the system to retrieve.
     * @return A weak pointer to the system if found, otherwise an expired weak pointer.
     */
	std::weak_ptr <System> GetSystem ( SystemType systemType )
	{
		return m_Systems . count ( systemType ) == 0 ? nullptr : m_Systems . at ( systemType );
	}

	/**
     * @brief Retrieves the SystemType code for the specified system type.
     * @tparam T The type of the system to get the SystemType code for.
     * @return The code representing the SystemType of the specified system.
     */
	template <typename T>
	SystemType GetSystemType () const
	{
		return typeid ( T ) . hash_code ();
	}

	/**
     * @brief Checks if a system of the specified type is registered.
     * @tparam T The type of the system to check for registration.
     * @return True if the system is registered, false otherwise.
     */
	template <typename T>
	bool GetIsSystemRegistered () const
	{
		const auto SystemType = GetSystemType <T> ();
		return GetIsSystemRegistered ( SystemType );
	}

	/**
     * @brief Checks if a system with the given SystemType is registered.
     * @param systemType The SystemType to check for registration.
     * @return True if a system with the specified SystemType is registered, false otherwise.
     */
	bool GetIsSystemRegistered ( SystemType systemType ) const
	{
		return m_Systems . count ( systemType ) != 0;
	}

	/**
     * @brief Notifies all systems of an entity's signature change.
     * @param entity The entity whose signature has changed.
     * @param signature The new signature of the entity.
     */
	void OnEntitySignatureChanged ( Entity & entity, const Signature & signature )
	{
		for ( auto & System : m_Systems ) {
			System . second -> OnEntitySignatureChanged ( entity, signature );
		}
	}

	/**
     * @brief Notifies all systems that an entity has been removed.
     * @param entity The entity that has been removed.
     */
	void OnEntityRemoved ( Entity & entity )
	{
		for ( auto & System : m_Systems ) {
			System . second -> RemoveEntityChecked ( entity );
		}
	}
	/* End SystemManager interface */

	/* Begin SystemManager safe interface */

	/**
     * @brief Registers a new system with the specified signature in a safe manner.
     * @tparam T The type of the system to be registered.
     * @param SystemSignature The signature of the system to be registered.
     * @return True if the system was successfully registered, false if the system is already registered.
     */
	template <typename T>
	bool RegisterSystemChecked ( Signature && SystemSignature )
	{
		if ( GetIsSystemRegistered <T> () )
			return false;
		RegisterSystem <T> ( std::forward <Signature> ( SystemSignature ) );
		return true;
	}

	/**
     * @brief Runs the specified system for the provided ECS instance in a safe manner.
     * @tparam T The type of the system to be run.
     * @param ecs The ECS instance to run the system on.
     * @return True if the system was successfully run, false if the system is not registered or has expired.
     */
	template <typename T>
	bool RunSystemChecked ( ECS & ecs )
	{
		std::shared_ptr <T> System = GetSystem <T> () . lock ();
		if ( ! System )
			return false;
		System -> Run ( ecs );
		return true;
	}
	/* End SystemManager safe interface */
private:

	std::unordered_map <SystemType, std::shared_ptr <System>> m_Systems; /**< The storage for registered systems. */
};