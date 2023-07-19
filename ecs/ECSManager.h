#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECSManager {

public:

    EntityHandle CreateEntity()
    {
        return m_EntityManager.CreateEntity ();
    }

    bool RemoveEntity ( EntityHandle entityHandle )
    {
        Entity & e = m_EntityManager.GetEntity( entityHandle );
        m_ComponentManager .OnEntityRemoved( e );
        return m_EntityManager.RemoveEntityChecked( entityHandle );
    }

    Entity & GetEntity ( EntityHandle entityHandle )
    {
        return m_EntityManager . GetEntity ( entityHandle );
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entityHandle, const ComponentType && component )
    {
        Entity & e = GetEntity( entityHandle );
        return m_ComponentManager.AddComponent ( e, component );
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entityHandle, const ComponentType & component )
    {
        Entity & e = GetEntity( entityHandle );
        return m_ComponentManager.AddComponent ( e, component );
    }

    template <typename ComponentType>
    bool RemoveComponent ( EntityHandle entityHandle )
    {
        Entity & e = GetEntity( entityHandle );
        return m_ComponentManager.RemoveComponent<ComponentType> ( e );
    }

    template<typename ComponentType>
    ComponentType & GetComponent ( EntityHandle entity )
    {
        Entity & e = GetEntity( entity );
        return m_ComponentManager.GetComponent<ComponentType>( e );
    }
    template <typename ComponentType>
    ComponentType GetComponentType ()
    {
        return m_ComponentManager.GetComponentType<ComponentType>();
    }

    template <typename ComponentType>
    bool RegisterComponent ()
    {
        return m_ComponentManager.RegisterComponent<ComponentType>();
    }
private:

    ComponentManager m_ComponentManager;
    EntityManager m_EntityManager;
    SystemManager m_SystemManager;
};
