#pragma once
#include "ObjectManager.h"
#include "EntityAction.h"

class EntityActionManager : public ObjectManager {

public:
    std::vector <EntityAction*> GetAllVisibleActions () const;
};