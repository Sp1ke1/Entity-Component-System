#include "EntityActionManager.h"


std::vector<EntityAction *> EntityActionManager::GetAllVisibleActions() const
{
    std::vector <EntityAction*> Out;
    for ( auto Object  : m_Objects )
    {
        auto EntityAction = static_cast <class EntityAction*> ( Object );
        if ( EntityAction && EntityAction -> CanBeVisible() )
        {
            Out.push_back( EntityAction );
        }
    }
    return Out;
}
