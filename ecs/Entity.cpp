#include "Entity.h"


Entity::Entity ( EntityIdType id )
: m_ID ( id ) {}

const EntityIdType Entity::GetId () const
{
    return m_ID;
}