#include "EntityActionManager.h"


std::vector<EntityAction *> EntityActionManager::GetAllVisibleActions() const
{
    std::vector <EntityAction*> Out;
    for ( const auto & object : m_Objects  )
    {
        if ( object && object -> CanBeVisible()  )
        {
            Out.push_back( object );
        }
    }
    return Out;
}
