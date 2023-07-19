#include "Entity.h"


Entity::Entity ( EntityHandle id )
: m_Id ( id ) {}

const EntityHandle Entity::GetId () const
{
    return m_Id;
}