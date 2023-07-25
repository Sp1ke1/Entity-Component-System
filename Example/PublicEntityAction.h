#pragma once

#include "EntityAction.h"
#include <iostream>

class PublicEntityAction : public EntityAction
{
public:
	PublicEntityAction ( EntityHandle owner, std::size_t id )
			: EntityAction ( owner, id )
	{};

	virtual bool CanBeVisible () const override { return m_IsVisible; }
	virtual void SetVisible ( bool isVisible ) override { m_IsVisible = isVisible; }
	virtual void Action () override { std::cout << "Public Action. Owner: " << GetOwner() << " Id: " << GetId() << " making action" << std::endl; }

private:
	bool m_IsVisible = true;
};