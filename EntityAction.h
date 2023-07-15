#pragma once
#include "Object.h"

class EntityAction : public Object<EntityAction> {
public:
    explicit EntityAction ( Entity<EntityAction> * Owner );

    virtual bool CanBeVisible() const;

    virtual void SetIsVisible( bool IsVisible );

private:
    bool m_IsVisible = false;
};
