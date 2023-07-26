#pragma once

#include "cstdint"
#define NULL_HANDLE (-1ULL)


typedef std::size_t ObjectHandle;
typedef ObjectHandle EntityHandle;
typedef ObjectHandle SystemHandle;
typedef ObjectHandle ComponentHandle;
typedef std::size_t ComponentType;
typedef std::size_t SystemType;


struct ComponentInfo
{
	ComponentType Type;
	ComponentHandle Handle;

	bool operator < ( const ComponentInfo & rhs ) const
	{ return Type < rhs . Type; };

	bool operator == ( const ComponentInfo & rhs ) const
	{ return Type == rhs . Type; };
};

