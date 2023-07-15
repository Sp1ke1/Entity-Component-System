#pragma once

#include <vector>



class Object;
class ObjectManager {

public:

    bool RegisterObject ( Object * object );

    bool UnregisterObject ( Object * object );

protected:
    std::vector<Object*> m_Objects;
};




