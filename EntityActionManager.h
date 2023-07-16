#pragma once
#include "ecs/ObjectManager.h"
#include "EntityAction.h"

class EntityActionManager : public ObjectManager<EntityAction> {

public:
    std::vector <EntityAction*> GetAllVisibleActions () const;
};