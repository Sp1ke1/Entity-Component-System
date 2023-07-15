#pragma once
#include "ObjectManager.h"

typedef unsigned long EntityIdType;

class Entity {
public:

    Entity ( EntityIdType id, ObjectManager * manager );

    ObjectManager & GetObjectManager ();

    const ObjectManager & GetObjectManager () const;

private:
    EntityIdType m_ID;
    ObjectManager * m_ObjectManager;
};

