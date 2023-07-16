#include "PackedComponentArray.h"
#include <unordered_map>
#include <memory>

// Reference: https://austinmorlan.com/posts/entity_component_system/#the-component-array

class ComponentManager  {

public:
    template <typename ComponentType>
    bool RegisterComponent ()
    {
        ComponentType TypeId = typeid ( ComponentType ).hash_code();
        if ( m_RegisteredTypes . count ( TypeId ) != 0 )
            return false;
        m_RegisteredTypes [ TypeId ] = m_RegisteredComponentCounter++;
        m_Components [ TypeId ] = std::make_shared <ComponentType> ();
        return true;
    }

    template <typename ComponentType>
    bool AddComponent ( EntityHandle entity, const ComponentType & component )
    {
        return GetComponentArray<ComponentType>() -> AddComponent ( entity, component );
    }

    template <typename ComponentType>
    bool RemoveComponent ( EntityHandle entity )
    {
        return GetComponentArray<ComponentType>() -> RemoveComponent ( entity );
    }

    void OnEntityRemoved ( EntityHandle entity )
    {
        for ( const auto & Array : m_Components )
        {
            Array.second ->OnEntityRemoved( entity );
        }
    }

private:
    template <typename ComponentType>
    std::shared_ptr<PackedArray<ComponentType>> GetComponentArray ()
    {
        const ComponentType TypeId = typeid ( ComponentType ).hash_code();
        return m_Components . at ( TypeId ); // TODO: will throw an exception if not registered type
    }
    using ComponentType = std::size_t;

    std::unordered_map <ComponentType, std::shared_ptr<IPackedComponentsArray>> m_Components;
    std::unordered_map <ComponentType, ComponentType> m_RegisteredTypes;
    ComponentType  m_RegisteredComponentCounter = 0;

};
