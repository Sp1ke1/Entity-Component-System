#pragma once
#include "Object.h"

class EntityAction : public Object {
public:
    explicit EntityAction ( Entity * Owner );

    virtual bool CanBeVisible() const;

    virtual void SetIsVisible( bool IsVisible );

private:
    bool m_IsVisible = false;
};
