#pragma once

#include "ECSInclude.h"

using Signature = std::set<ComponentType>;

class Entity {

public:

    explicit Entity(EntityHandle id)
            : m_Handle(id) {};

    // --- Begin entity interface

    const EntityHandle GetHandle() const {
        return m_Handle;
    }

    void AddComponent(ComponentInfo type) {
        m_Components.insert(type);
    }

    void RemoveComponent(ComponentType type) {
        m_Components.erase({type, 0});
    }

    bool GetHasComponent(ComponentType type) const {
        return m_Components.count({type, 0}) != 0;
    }

    ComponentHandle GetComponentHandle(ComponentType type) const {
        return m_Components.find({type, 0})->Handle;
    }

    const std::set<ComponentInfo> &GetComponentsInfo() const {
        return m_Components;
    }

    Signature GetSignature() const {
        Signature out;
        for (const auto &elem: m_Components) {
            out.insert(elem.Type);
        }
        return out;
    }
    // --- End entity interface

    // --- Begin entity safe interface
    std::optional<ComponentHandle> GetComponentHandleChecked(ComponentType type) const {
        if (!GetHasComponent(type))
            return std::nullopt;
        return GetComponentHandle(type);
    }
    // --- End entity safe interface

private:
    EntityHandle m_Handle;
    std::set<ComponentInfo> m_Components;
};

