#pragma once

#include "ECS/ComponentBase.h"
#include "Types.h"

class Object : public ComponentBase
{
public:

	explicit Object ( EntityHandle owner, std::size_t id ) : ComponentBase ( owner ), m_Id ( id )
	{};

	std::size_t GetId () const
	{ return m_Id; }

private:
	std::size_t m_Id;
};
