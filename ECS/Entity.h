#pragma once

#include "Types.h"

using Signature = std::set<ComponentType>;


/**
 * @class Entity
 * @brief Represents an entity in the system with attached components.
 *
 * The Entity class represents an entity in the system that can have various components attached to it.
 * It provides methods to add, remove, and query components information associated with the entity.
 */
class Entity {

public:

    /**
     * @brief Constructor for Entity.
     * @param id The handle of the entity.
     */
    explicit Entity(EntityHandle id)
            : m_Handle(id) {};

    // --- Begin entity interface

    /**
    * @brief Get the handle of the entity.
    * @return The handle of the entity.
    */
	EntityHandle GetHandle() const {
        return m_Handle;
    }

    /**
     * @brief Add a component to the entity.
     * @param type The information of the component to add.
     */
    void AddComponent(ComponentInfo componentInfo) {
        m_Components.insert(componentInfo);
    }

    /**
     * @brief Remove a component from the entity.
     * @param type The type of the component to remove.
     */
    void RemoveComponent(ComponentType type) {
        m_Components.erase({type, NULL_HANDLE});
    }

    /**
     * @brief Check if the entity has a specific component.
     * @param type The type of the component to check.
     * @return True if the entity has the component, false otherwise.
     */
    bool GetHasComponent(ComponentType type) const {
        return m_Components.count({type, NULL_HANDLE}) != 0;
    }

    /**
     * @brief Get the handle of a specific component attached to the entity.
     * @param type The type of the component to retrieve the handle for.
     * @return The handle of the specified component, or an invalid handle if not found.
     */
    ComponentHandle GetComponentHandle(ComponentType type) const {
        return m_Components.find({type, NULL_HANDLE})->Handle;
    }
    /**
     * @brief Get the set of component information attached to the entity.
     * @return A constant reference to the set of ComponentInfo.
     */
    const std::set<ComponentInfo> &GetComponentsInfo() const {
        return m_Components;
    }
    /**
     * @brief Get the signature of the entity based on its attached components.
     * @return The Signature of the entity.
     */
    Signature GetSignature() const {
        Signature out;
        for (const auto &elem: m_Components) {
            out.insert(elem.Type);
        }
        return out;
    }
    // --- End entity interface


    // --- Begin entity safe interface
    /**
     * @brief Get the handle of a specific component attached to the entity (safe version).
     * @param type The type of the component to retrieve the handle for.
     * @return An optional ComponentHandle.

     * This is a safer alternative to GetComponentHandle() method when it is uncertain whether the component is attached to the entity.
     */
    std::optional<ComponentHandle> GetComponentHandleChecked(ComponentType type) const {
        if (!GetHasComponent(type))
            return std::nullopt;
        return GetComponentHandle(type);
    }
    // --- End entity safe interface

private:
    EntityHandle m_Handle; /**< The handle of the entity. */
    std::set<ComponentInfo> m_Components; /**< Set of components information associated with the entity. */
};

