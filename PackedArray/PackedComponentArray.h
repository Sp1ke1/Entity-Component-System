#include "PackedArray.h"
#include "Types.h"
#include <unordered_map>


class IPackedComponentsArray {

public:
    virtual void OnEntityRemoved ( EntityHandle handle ) = 0;
};

// Reference: https://austinmorlan.com/posts/entity_component_system/#the-component-array

template <typename ComponentType>
class PackedComponentArray : public IPackedComponentsArray
{
public:
    bool AddComponent ( EntityHandle entity, const ComponentType & component )
    {
        ComponentHandle handle = m_PackedComponentsArray . Add (component );
        m_EntityComponentMap [ entity ] = handle;
    }

    bool RemoveComponent ( EntityHandle entity )
    {
        if ( m_EntityComponentMap . count ( entity ) == 0 )
            return false;
        return m_PackedComponentsArray . Remove (m_EntityComponentMap . at (entity ) );
    }

    bool GetComponent ( EntityHandle entity, ComponentType & out )
    {
        if ( m_EntityComponentMap.count ( entity ) == 0 )
            return false;
        return m_PackedComponentsArray . Get (m_EntityComponentMap . at (entity ), out );
    }

    void OnEntityRemoved ( EntityHandle handle ) override
    {
        RemoveComponent( handle );
    }

private:
    std::unordered_map <EntityHandle, ComponentHandle> m_EntityComponentMap;
    PackedArray <ComponentType> m_PackedComponentsArray;
};

