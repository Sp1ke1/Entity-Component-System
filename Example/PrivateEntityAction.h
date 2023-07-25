#pragma once

#include "EntityAction.h"
#include <iostream>

class PrivateEntityAction : public EntityAction
{
public:
	PrivateEntityAction ( EntityHandle owner, std::size_t id )
	: EntityAction ( owner, id )
	{};

	virtual bool CanBeVisible () const override { return m_Visible; }
	virtual void SetVisible ( bool isVisible ) override { m_Visible = isVisible; }
	virtual void Action () override { std::cout << "Private Action. Owner: " << GetOwner() << " Id: " << GetId() << " making action" << std::endl; }

private:
	bool m_Visible = false;
};