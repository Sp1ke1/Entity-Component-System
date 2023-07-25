#pragma once

#include <ECSInclude.h>

class ComponentBase
{

public:
	ComponentBase ( EntityHandle owner ) : m_Owner ( owner )
	{};

	EntityHandle GetOwner () const
	{ return m_Owner; }

private:
	EntityHandle m_Owner;
};
