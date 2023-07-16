#pragma once
#include "Object.h"

class EntityAction : public Object {
public:


    virtual bool CanBeVisible() const;

    virtual void SetIsVisible( bool IsVisible );

private:
    bool m_IsVisible = false;
};
