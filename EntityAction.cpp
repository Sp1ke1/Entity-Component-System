#include "EntityAction.h"

bool EntityAction::CanBeVisible() const {
    return m_IsVisible;
}

void EntityAction::SetIsVisible(bool IsVisible) {
    m_IsVisible = IsVisible;
}
