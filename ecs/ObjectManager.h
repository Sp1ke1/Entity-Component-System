#pragma once

#include "PackedArray.h"
#include "Types.h"

template <typename ObjectType>
class ObjectManager {

public:

    template <typename ... Args>
    ObjectHandle CreateObject ( Args && ... )
    {
        ObjectType obj ( Args ... );
        return m_Objects . Add ( std::move ( obj ) );
    }

    bool GetObject ( ObjectHandle handle, ObjectType & out ) const
    {
        return m_Objects.Get ( handle, out );
    }

    bool RemoveObject ( ObjectHandle handle )
    {
        return m_Objects.Remove( handle );
    }

    template <typename MethodName, typename ... Args>
    void CallObjectsMethod( Args && ... MethodArguments ) const
    {
        for ( auto It = m_Objects.begin(); It != m_Objects.end(); ++It )
        {
            It -> template MethodName ( std::forward <Args> ( MethodArguments ) ... );
        }
    }

    void Clear ()
    {
        m_Objects.Clear();
    }

protected:
    PackedArray <ObjectType> m_Objects;
};




