#pragma once

#include "Types.h"

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
