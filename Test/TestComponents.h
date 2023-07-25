#pragma once

#include <iostream>
#include "ComponentBase.h"


struct Vector
{
	Vector ( float x, float y, float z )
			: X ( x ), Y ( y ), Z ( z )
	{};

	Vector operator * ( float rhs ) const
	{ return Vector ( X * rhs, Y * rhs, Z * rhs ); }

	Vector operator + ( const Vector & rhs ) const
	{ return { X + rhs . X, Y + rhs . Y, Z + rhs . Z }; }

	const Vector & operator += ( const Vector & rhs )
	{
		X += rhs . X;
		Y += rhs . Y;
		Z += rhs . Z;
		return * this;
	}


    bool operator != ( const Vector & rhs ) const { return X != rhs . X || Y != rhs . Y || Z != rhs . Z; }
    bool operator == ( const Vector & rhs ) const { return ! (rhs != *this); }

	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
};

struct LocationComponent : public ComponentBase
{
	LocationComponent ( EntityHandle owner, Vector location )
			: ComponentBase ( owner ), Location ( location )
	{};
	Vector Location;
};

struct MovementComponent : public ComponentBase
{
	MovementComponent ( EntityHandle owner, float speed, Vector direction )
			: ComponentBase ( owner ), Speed ( speed ), Direction ( direction )
	{};
	float Speed = 0.f;
	Vector Direction;
};

struct HPComponent : public ComponentBase
{
	HPComponent ( EntityHandle owner, int hp, bool isDead = false )
			: ComponentBase ( owner ), HP ( hp ), IsDead ( isDead )
	{};

	void ReceiveDamage ( int damage )
	{
		HP -= damage;
		if ( HP <= 0 ) {
			HP = 0;
			IsDead = true;
			return;
		}
    }

	int HP = 100;
	bool IsDead = false;
};

struct DamageComponent : public ComponentBase
{
	DamageComponent ( EntityHandle owner, EntityHandle entityToDamage, int damage )
			: ComponentBase ( owner ), EntityToDamage ( entityToDamage ), Damage ( damage )
	{};


	void Attack ( HPComponent & other )
	{
		if ( other . GetOwner () != EntityToDamage ) {
			return;
		}
		other . ReceiveDamage ( Damage );
	}

	EntityHandle EntityToDamage;
	int Damage = 10;
};