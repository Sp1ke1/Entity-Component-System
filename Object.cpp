
#include "Object.h"
#ifdef ECS_EXCEPTIONS
#include "stdexcept"
#endif


Object::Object(Entity *Owner)
: m_Owner ( Owner ) {


    if ( !m_Owner )
    {
        #ifdef ECS_EXCEPTIONS
        throw std::invalid_argument ("Creating object with invalid Entity owner");
        #endif
        return;
    }
    m_Owner -> GetObjectManager().RegisterObject( this );
}

Object::~Object() {
    if ( m_Owner )
    {
        m_Owner -> GetObjectManager().UnregisterObject( this );
    }
}

void Object::OnRemoved() {
    if ( m_Owner )
    {
        m_Owner -> GetObjectManager().UnregisterObject( this );
        m_Owner = nullptr;
    }

}
