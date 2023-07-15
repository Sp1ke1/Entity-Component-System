#pragma once
#include "ObjectManager.h"

#ifdef ECS_EXCEPTIONS
#include <stdexcept>
#endif

typedef unsigned long EntityIdType;

template <typename ObjectType>
class Entity {
public:

    explicit Entity ( EntityIdType id )
    : m_ID ( id ), m_ObjectManager ( new ObjectManager<ObjectType>() )
    {
        if ( !m_ObjectManager )
        {
            #ifdef ECS_EXCEPTIONS
            throw std::invalid_argument ( "Invalid manager in Entity constructor" );
            #endif
            return;
        }
    }

    ~Entity ()
    {
        delete m_ObjectManager;
        m_ObjectManager = nullptr;
    }

    const EntityIdType GetEntityID () const
    {
        return m_ID;
    }

    ObjectManager<ObjectType> & GetObjectManager ()
    {
        return *m_ObjectManager;
    }

    const ObjectManager<ObjectType> & GetObjectManager () const
    {
        return *m_ObjectManager;
    }

private:
    EntityIdType m_ID;
    ObjectManager <ObjectType> * m_ObjectManager;
};

