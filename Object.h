#pragma once
#include "Types.h"

class Object {
public:

    explicit Object ( EntityHandle owner ) : m_Owner ( owner )
    {};

    EntityHandle GetOwner () const
    {
        return m_Owner;
    }

    void SetOwner ( EntityHandle entity )
    {
        m_Owner = entity;
    }

private:
    EntityHandle m_Owner;
};
