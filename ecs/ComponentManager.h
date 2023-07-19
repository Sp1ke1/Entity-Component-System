#include "ObjectManager.h"
#include "Entity.h"
#include <unordered_map>
#include <memory>

#include <iostream>

// Reference: https://austinmorlan.com/posts/entity_component_system/#the-component-array

class ComponentManager  {

public:
    template <typename T>
    bool RegisterComponent ()
    {
        auto ComponentType = typeid ( T ).hash_code();
        if ( m_RegisteredTypes . count ( ComponentType ) != 0 )
            return false;
        m_RegisteredTypes [ ComponentType ] = m_RegisteredComponentCounter;
        m_Components [ m_RegisteredComponentCounter ] = std::make_shared <ObjectManager<T>> ();
        m_RegisteredComponentCounter++;
        return true;
    }

    template <typename T>
    bool AddComponent ( Entity & entity, const T & component ) // TODO: make checks if entity already has component
    {
        const auto ComponentHandle =  GetComponentArray<T>() -> AddObject ( component );
        entity .AddComponent( { GetComponentType<T>(), ComponentHandle } );
        return true;
    }

    template <typename T>
    bool RemoveComponent ( Entity & entity )
    {
        const auto ComponentType = GetComponentType<T>();
        const auto ComponentHandle = entity.GetComponentHandle( ComponentType );

        auto ComponentArray = GetComponentArray<T>();

        if ( !ComponentArray -> RemoveObjectChecked ( ComponentHandle ) )
            return false;

        entity . RemoveComponent ( ComponentType );
        return true;
    }

    template <typename T>
    T & GetComponent ( Entity & entity )
    {
        const auto ComponentHandle =  entity.GetComponentHandle(GetComponentType<T>());
        auto ComponentArray = GetComponentArray<T>();
        return ComponentArray -> GetObject ( ComponentHandle );
    }

    template <typename T>
    std::optional<std::reference_wrapper<T>> GetChecked ( Entity & entity )
    {
        // TODO: blows if component isn't registered
        ComponentHandle handle;
        if ( !entity.GetComponentHandle (GetComponentType<T>()) )
            return std::nullopt;
        return GetComponentArray<T>() -> GetObjectChecked ( handle );
    }

    void OnEntityRemoved ( Entity & entity )
    {
        const auto & ComponentInfo = entity.GetComponentsInfo();
        for ( const auto & info : ComponentInfo )
        {
            m_Components . at ( info.Type )->RemoveObjectChecked( info.Handle );
        }
    }

    template <typename T>
    ComponentType GetComponentType () const
    {
        const auto ComponentType = typeid ( T ).hash_code();
        return m_RegisteredTypes . at ( ComponentType );
    }

    template <typename T>
    std::weak_ptr <ObjectManager<T>> GetComponentsByType ()
    {
        return GetComponentArray<T>();
    }

private:
    template <typename T>
    std::shared_ptr<ObjectManager<T>> GetComponentArray ()
    {
        const auto RegisteredComponentType = GetComponentType<T>();
        return GetComponentArray<T> ( RegisteredComponentType );
    }

    template <typename T>
    std::shared_ptr<ObjectManager<T>> GetComponentArray ( ComponentType componentType )
    {
        auto Interface = m_Components . at ( componentType );
        return std::static_pointer_cast<ObjectManager<T>> ( Interface );
    }



    std::unordered_map <ComponentType, std::shared_ptr<IObjectManager>> m_Components;
    std::unordered_map <ComponentType, ComponentType> m_RegisteredTypes;
    ComponentType  m_RegisteredComponentCounter = 0;

};
