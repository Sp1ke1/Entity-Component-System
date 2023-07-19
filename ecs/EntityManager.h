#include "ObjectManager.h"
#include "Entity.h"
#include <functional>

class EntityManager  {

public:
    EntityHandle CreateEntity ()
    {
        return m_Entities.CreateObject( HandleInjectionMethod::Constructor );
    }

    Entity & GetEntity ( EntityHandle entityHandle )
    {
        return m_Entities.GetObject( entityHandle );
    }

    std::optional<std::reference_wrapper<Entity>> GetEntityChecked ( EntityHandle entityHandle )
    {
        return m_Entities.GetObjectChecked( entityHandle  );
    }

    void RemoveEntity ( EntityHandle entityHandle )
    {
        m_Entities.RemoveObject( entityHandle );
    }

    bool RemoveEntityChecked ( EntityHandle entityHandle )
    {
        return m_Entities.RemoveObjectChecked( entityHandle );
    }

    void RemoveAllEntities ()
    {
        m_Entities.Clear();
    }

    bool GetEntityHasComponent ( EntityHandle handle, ComponentType componentType )
    {
        auto obj = GetEntityChecked( handle );
        if ( !obj.has_value() )
            return false;
        return obj.value().get().GetHasComponent( handle );
    }

private:

    ObjectManager <Entity> m_Entities;
};

