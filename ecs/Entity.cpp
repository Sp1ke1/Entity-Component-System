#include "Entity.h"


Entity::Entity ( EntityHandle id )
: m_Handle (id ) {}

const EntityHandle Entity::GetHandle () const
{
    return m_Handle;
}