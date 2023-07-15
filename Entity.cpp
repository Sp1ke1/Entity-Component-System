#include "Entity.h"

#ifdef ECS_EXCEPTIONS
#include <stdexcept>
#endif

ObjectManager & Entity::GetObjectManager() {
    return *m_ObjectManager;
}

const ObjectManager & Entity::GetObjectManager() const {
    return *m_ObjectManager;
}

Entity::Entity(EntityIdType id, ObjectManager *manager)
: m_ID ( id ), m_ObjectManager ( manager ) {
    if ( !m_ObjectManager )
    {
        #ifdef ECS_EXCEPTIONS
        throw std::invalid_argument ( "Invalid manager in Entity constructor" );
        #endif
        return;
    }
}

