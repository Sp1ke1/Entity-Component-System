#pragma once

#include "cstdint"
#define NULL_HANDLE (-1ULL)


typedef std::size_t ObjectHandle;
typedef ObjectHandle EntityHandle;
typedef ObjectHandle SystemHandle;
typedef ObjectHandle ComponentHandle;
typedef std::size_t ComponentType;
typedef std::size_t SystemType;


/**
 * @brief Struct representing information about a component attached to an entity.
 */
struct ComponentInfo
{
	ComponentType Type; /** < The type identifier of the component. */
	ComponentHandle Handle; /** < The handle of the component within its respective ObjectManager. */

	bool operator < ( const ComponentInfo & rhs ) const
	{ return Type < rhs . Type; };

	bool operator == ( const ComponentInfo & rhs ) const
	{ return Type == rhs . Type; };
};

