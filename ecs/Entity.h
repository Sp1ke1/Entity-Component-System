#pragma once
#include "Types.h"
#include <set>

class Entity {

public:

    explicit Entity ( EntityHandle id );

    Entity () = default;

    const EntityHandle GetId () const;

    void AddComponent ( ComponentInfo type )
    {
        m_Components.insert ( type );
    }

    void RemoveComponent ( ComponentType type )
    {
        auto Found = m_Components . find ( { type, 0 } );
        if ( Found != m_Components . end() )
        {
            m_Components . erase ( Found );
        }
    }

    bool GetHasComponent ( ComponentType type ) const
    {
        return m_Components.count ( { type, 0 } ) != 0;
    }

    ComponentHandle GetComponentHandle ( ComponentType type ) const
    {
        return m_Components . find ( { type, 0 } ) -> Handle;
    }

    bool GetComponentHandleChecked ( ComponentType type, ComponentHandle & out ) const
    {
        auto Found = m_Components . find ( { type, 0 } );
        if ( Found == m_Components . end() )
            return false;
        return Found -> Handle;
    }

    const std::set<ComponentInfo> & GetComponentsInfo () const
    {
        return m_Components;
    }



private:
    EntityHandle m_Id;
    std::set <ComponentInfo> m_Components;
};

