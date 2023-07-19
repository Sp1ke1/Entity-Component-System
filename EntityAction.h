#pragma once
#include "Object.h"

class EntityAction : public ObjectComponent {
public:


    virtual bool CanBeVisible() const;

    virtual void SetIsVisible( bool IsVisible );

private:
    bool m_IsVisible = false;
};
