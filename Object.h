#pragma once

#include "Entity.h"

#ifdef ECS_EXCEPTIONS
#include "stdexcept"
#endif

template <typename Derived>
class Object {
public:

    // CRTP
    explicit Object ( Entity<Derived> * Owner )
    : m_Owner ( Owner )
    {
        if ( !m_Owner )
        {
            #ifdef ECS_EXCEPTIONS
            throw std::invalid_argument ("Creating object with invalid Entity owner");
            #endif
            return;
        }
        m_Owner -> GetObjectManager().RegisterObject( static_cast <Derived*> ( this ) );
    }

    Object () = delete;

    ~Object ()
    {
        if ( m_Owner )
        {
            m_Owner -> GetObjectManager().UnregisterObject( static_cast<Derived*> ( this ) );
            m_Owner = nullptr;
        }
    }

    void OnRemoved ()
    {
        if ( m_Owner )
        {
            m_Owner -> GetObjectManager().UnregisterObject( static_cast <Derived*> ( this ) );
            m_Owner = nullptr;
        }
    }

private:
    Entity<Derived> * m_Owner;
};
