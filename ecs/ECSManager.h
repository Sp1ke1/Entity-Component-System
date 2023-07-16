#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECSManager {


    template <typename ... Args>
    EntityHandle CreateEntity( Args && ... Arguments )
    {
        return m_EntityManager.CreateObject( std::forward<Args> ( Arguments ) ... );
    }

    bool RemoveEntity ( EntityHandle entityHandle )
    {
        return m_EntityManager.RemoveObject( entityHandle );
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entityHandle, const ComponentType && component )
    {
        return m_ComponentManager.AddComponent ( entityHandle, component );
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entityHandle, const ComponentType & component )
    {
        return m_ComponentManager.AddComponent ( entityHandle, component );
    }

    template <typename ComponentType>
    bool RemoveComponent ( EntityHandle entityHandle )
    {
        return m_ComponentManager.RemoveComponent<ComponentType> ( entityHandle  );
    }


    ComponentManager m_ComponentManager;
    EntityManager m_EntityManager;
    SystemManager m_SystemManager;
};
