#include "ObjectManager.h"
#include <algorithm>

bool ObjectManager::RegisterObject(Object *object) {
    if ( !object )
        return false;
    m_Objects . push_back ( object );
    return true;
}

bool ObjectManager::UnregisterObject(Object *object) {
    if ( !object )
        return false;

    auto FoundObject = std::find ( m_Objects.begin(), m_Objects.end(), object );
    if ( FoundObject == m_Objects.end() )
        return false;
    m_Objects.erase ( FoundObject );
    return true;
}
