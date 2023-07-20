#include "EntityManager.h"
#include "ComponentManager.h"
#include "SystemManager.h"

class ECS {

public:

    EntityHandle CreateEntity()
    {
        return m_EntityManager.CreateEntity ();
    }

    bool RemoveEntity ( EntityHandle entityHandle )
    {
        Entity & e = m_EntityManager.GetEntity( entityHandle );
        m_ComponentManager .OnEntityRemoved( e );
        m_SystemManager . OnEntityRemoved( e );
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
        if ( !m_ComponentManager.AddComponent ( e, component ) )
            return false;
        m_SystemManager.OnEntitySignatureChanged( e, e.GetSignature() );
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entityHandle, const ComponentType & component )
    {
        Entity & e = GetEntity( entityHandle );
        if ( !m_ComponentManager.AddComponent ( e, component ) )
            return false;
        m_SystemManager.OnEntitySignatureChanged( e, e.GetSignature() );
    }

    template <typename ComponentType>
    bool RemoveComponent ( EntityHandle entityHandle )
    {
        Entity & e = GetEntity( entityHandle );
        if ( ! m_ComponentManager.RemoveComponent<ComponentType> ( e ) )
            return false;
        m_SystemManager.OnEntitySignatureChanged( e, e.GetSignature() );
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

    template <typename System, typename ... ComponentTypes>
    bool RegisterSystem ()
    {
        return m_SystemManager . RegisterSystem<System>( std::move ( GetComponentsSignature <ComponentTypes> () ... ) );
    }

    template <typename ... ComponentTypes>
    std::set<ComponentType> GetComponentsSignature ()
    {
        return { m_ComponentManager.GetComponentType<ComponentTypes>() ... };
    }

    template <typename System>
    void RunSystem ()
    {
        m_SystemManager.RunSystem<System>( this );
    }
private:

    ComponentManager m_ComponentManager;
    EntityManager m_EntityManager;
    SystemManager m_SystemManager;

};
