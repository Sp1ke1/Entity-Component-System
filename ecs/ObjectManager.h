#pragma once

#include "PackedArray.h"



class IObjectManager
{
public:
    virtual ~IObjectManager() = default;
    virtual bool RemoveObjectChecked (ObjectHandle handle ) = 0;
    virtual void RemoveObject ( ObjectHandle handle ) = 0;
    virtual void Clear () = 0;
};

template <typename ObjectType>
class ObjectManager : public IObjectManager {

public:

    template <typename ... Args>
    ObjectHandle CreateObject ( HandleInjectionMethod injectionMethod = NoInjection, Args && ... Arguments )
    {
        return m_Objects . EmplaceBack ( injectionMethod, std::forward <Args> ( Arguments ) ... );
    }

    ObjectHandle AddObject ( const ObjectType & object )
    {
        return m_Objects.Add ( object );
    }

    ObjectHandle AddObject ( const ObjectType && object )
    {
        return m_Objects.Add ( object );
    }

    ObjectType & GetObject ( ObjectHandle handle )
    {
        return m_Objects.Get ( handle );
    }

    std::optional<std::reference_wrapper<ObjectType>> GetObjectChecked (ObjectHandle handle )
    {
        return m_Objects.GetChecked(handle);
    }

    bool RemoveObjectChecked (ObjectHandle handle ) override
    {
        return m_Objects.RemoveChecked(handle);
    }

    void RemoveObject ( ObjectHandle handle ) override
    {
        return m_Objects.Remove ( handle );
    }

    bool GetIsValidHandle ( ObjectHandle handle ) const
    {
        return m_Objects.IsValidHandle( handle  );
    }

    void Clear () override
    {
        m_Objects.Clear();
    }

    typename std::vector<ObjectType>::iterator begin()
    {
        return m_Objects.begin();
    }

    typename std::vector<ObjectType>::iterator end()
    {
        return m_Objects.end();
    }

protected:
    PackedArray <ObjectType> m_Objects;
};




