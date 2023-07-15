#pragma once

#include "Entity.h"

class Object {
public:
    explicit Object ( Entity * Owner );

    Object () = delete; 

    ~Object ();

    void OnRemoved ();

private:
    Entity * m_Owner;
};
