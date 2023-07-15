#pragma once

#include <algorithm>
#include <vector>

template <typename ObjectType>
class ObjectManager {

public:

    bool RegisterObject ( ObjectType * object )
    {
        if ( !object )
            return false;
        m_Objects . push_back ( object );
        return true;
    }

    bool UnregisterObject ( ObjectType * object )
    {
        if ( !object )
            return false;

        auto FoundObject = std::find ( m_Objects.begin(), m_Objects.end(), object );
        if ( FoundObject == m_Objects.end() )
            return false;
        m_Objects.erase ( FoundObject );
        return true;
    }

protected:
    std::vector<ObjectType*> m_Objects;
};




