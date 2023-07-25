#include "System.h"
#include "ECS.h"
#include "TestComponents.h"

class DamageSystem : public System
{
public:
	DamageSystem ( Signature && s ) : System ( std::move ( s ) )
	{};

	DamageSystem () = default;

	virtual void Run ( ECS & ecs ) override
	{

		// Deferred destroy to not brake m_Entities array.
		std::vector <EntityHandle> DeadEntitiesToRemove;
		std::vector <EntityHandle> DamageComponentsToRemove;
		for ( auto e : m_Entities ) {
			auto & damageComponent = ecs . GetComponent <DamageComponent> ( e );
			auto & hpComponent = ecs . GetComponent <HPComponent> ( damageComponent . EntityToDamage );

			damageComponent . Attack ( hpComponent );

			if ( hpComponent . IsDead ) {
				DamageComponentsToRemove . push_back ( e );
				DeadEntitiesToRemove . push_back ( damageComponent . EntityToDamage );
				continue;
			}
		}
		for ( auto e : DeadEntitiesToRemove ) {
			ecs . RemoveEntity ( e );
		}

		for ( auto e : DamageComponentsToRemove ) {
			ecs . RemoveEntity ( e );
		}
	}
};

class MovementSystem : public System
{
public:
	MovementSystem ( Signature && s ) : System ( std::move ( s ) )
	{};

	MovementSystem () = default;

	virtual void Run ( ECS & ecs ) override
	{
		for ( auto e : m_Entities ) {
			auto & movementComponent = ecs . GetComponent <MovementComponent> ( e );
			auto & locationComponent = ecs . GetComponent <LocationComponent> ( e );
			locationComponent . Location += movementComponent . Direction * movementComponent . Speed;
		}
	}
};

class RenderSystem : public System
{
public:

    virtual void Run ( ECS & ecs ) override
    {
    }
};

