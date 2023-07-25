#pragma once

#include "Object.h"

class EntityAction : public Object
{
public:

	EntityAction ( EntityHandle owner, std::size_t id )
			: Object ( owner, id )
	{};

	virtual ~EntityAction() = default;

	virtual bool CanBeVisible () const = 0;

	virtual void SetVisible ( bool isVisible ) = 0;

	virtual void Action () = 0;

};
