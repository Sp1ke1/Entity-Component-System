#pragma once
#include "ECSInclude.h"

using Signature = std::set<ComponentType>;

class Entity {

public:

    explicit Entity ( EntityHandle id );

    Entity () = default;

    const EntityHandle GetHandle () const;

    void AddComponent ( ComponentInfo type )
    {
        m_Components.insert ( type );
    }

    void RemoveComponent ( ComponentType type )
    {
        m_Components.erase ( { type, 0 } );
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

    Signature GetSignature () const
    {
        Signature out;
        for ( const auto & elem : m_Components )
        {
            out . insert ( elem.Type );
        }
        return out;
    }



private:
    EntityHandle m_Handle;
    std::set <ComponentInfo> m_Components;
};

