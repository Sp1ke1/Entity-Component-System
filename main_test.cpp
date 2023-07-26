#include <iostream>
#include "ECS.h"
#include "Test/TestSystems.h"
#include "Test/TestComponents.h"
#include <cassert>
#include <random>

#ifdef ECS_RELEASE
#error Tests are not supported in Release build configuration. Please use Debug configuration.
#endif


void TestPackedArray ()
{
	PackedArray <int> data;

	auto id1 = data . Add ( 3 );
	auto id2 = data . Add ( 4 );
	auto id3 = data . Add ( 10 );

	assert ( data . Size () == 3 );

	assert ( data . Get ( id1 ) == 3 );
	assert ( data . Get ( id2 ) == 4 );
	assert ( data . Get ( id3 ) == 10 );

	assert ( data . RemoveChecked ( id2 ) );

	assert ( data . IsValidHandle ( data . GetHandleFromIndex ( 1 ) ) );

	assert ( data . Size () == 2 );

	assert ( data . Get ( id1 ) == 3 );
	assert ( data . Get ( id3 ) == 10 );
	assert ( ! data . GetChecked ( id2 ) );

	auto id4 = data . Add ( 25 );

	assert ( data . Get ( id1 ) == 3 );
	assert ( data . Get ( id3 ) == 10 );
	assert ( data . Get ( id4 ) == 25 );

	data . Clear ();

	assert ( ! data . GetChecked ( id1 ) );
	assert ( ! data . GetChecked ( id3 ) );

	auto id5 = data . Add ( 8 );
	assert ( data . Get ( id5 ) == 8 );

	assert ( data . RemoveChecked ( id5 ) );
	assert ( ! data . GetChecked ( id5 ) );

	std::cout << "(Packed array) tests passed" << std::endl;

}

void TestECSComponentRegistration ( ECS & ecs )
{
	ecs . RegisterComponent <LocationComponent> ();
	assert ( ecs . GetIsComponentRegistered <LocationComponent> () );
	assert ( ecs . RegisterComponentChecked <MovementComponent> () );
	assert ( ecs . RegisterComponentChecked <DamageComponent> () );
	assert ( ecs . RegisterComponentChecked <HPComponent> () );
	assert ( ! ecs . RegisterComponentChecked <HPComponent> () );
	assert ( ecs . GetIsComponentRegistered <HPComponent> () );
	std::cout << "(Component registration) tests passed" << std::endl;
}

void TestECSSystemRegistration ( ECS & ecs )
{
	ecs . RegisterSystem <MovementSystem, LocationComponent, MovementComponent> ();
	assert ( ecs . GetIsSystemRegistered <MovementSystem> () );
	assert ( ! ecs . GetIsSystemRegistered <DamageSystem> () );
	bool Registered = ecs . RegisterSystemChecked <DamageSystem, DamageComponent, HPComponent> ();
	assert ( Registered );
	Registered = ecs . RegisterSystemChecked <DamageSystem, HPComponent, DamageComponent> ();
	assert ( ! Registered );
	assert ( ecs . GetIsSystemRegistered <DamageSystem> () );
	std::cout << "(System registration) tests passed" << std::endl;
}

void TestECSEntityCreation ( ECS & ecs )
{
	assert ( ! ecs . GetIsValidEntityHandle ( 0 ) );
	auto e1 = ecs . CreateEntity ();
	auto e2 = ecs . CreateEntity ();

	Entity & en1 = ecs . GetEntity ( e1 );
	auto en2 = ecs . GetEntityChecked ( e2 );
	assert ( en1 . GetHandle () == e1 );
	assert ( en2 );
	assert ( en2 -> get () . GetHandle () == e2 );
	assert ( ecs . GetIsValidEntityHandle ( e1 ) );
	assert ( ecs . GetIsValidEntityHandle ( e2 ) );
	ecs . RemoveEntity ( e1 );
	assert ( ! ecs . GetIsValidEntityHandle ( e1 ) );
	assert ( ecs . GetIsValidEntityHandle ( e2 ) );
	assert ( ! ecs . RemoveEntityChecked ( e1 ) );
	assert ( ecs . RemoveEntityChecked ( e2 ) );
	assert ( ! ecs . GetIsValidEntityHandle ( e2 ) );
	std::cout << "(Entity creation/deletion) tests passed" << std::endl;
}

void TestECSComponentsCreation ( ECS & ecs )
{
	auto e1 = ecs . CreateEntity ();
	auto e2 = ecs . CreateEntity ();
	auto e3 = ecs . CreateEntity ();

	ecs . AddComponent <LocationComponent> ( e1, { e1, { 0.0f, 0.0f, 1.0f } } );
	assert ( ecs . GetEntityHasComponent <LocationComponent> ( e1 ) );
	assert ( ecs . GetEntityHasComponentChecked <LocationComponent> ( e1 ) );
	auto ComponentOpt = ecs . GetComponentChecked <LocationComponent> ( e1 );
	assert ( ComponentOpt . has_value () );
	LocationComponent & Component = ComponentOpt -> get ();
	Component . Location . X = 10.f;
	auto Component1 = ecs . GetComponent <LocationComponent> ( e1 );
	assert ( Component1 . Location . X == 10.f );
	assert ( ! ecs . AddComponentChecked <LocationComponent> ( e1, { e1, { 0.0f, 0.0f, 0.0f } } ) );
	assert ( ecs . AddComponentChecked <HPComponent> ( e2, { e2, 100 } ) );
	assert ( ! ecs . GetEntityHasComponent <HPComponent> ( e1 ) );
	assert ( ecs . GetEntityHasComponent <HPComponent> ( e2 ) );
	ecs . AddComponent <LocationComponent> ( e3, { e3, { 10.f, 10.f, 10.f } } );
	ecs . RemoveComponent <LocationComponent> ( e1 );
	assert ( ! ecs . GetEntityHasComponent <LocationComponent> ( e1 ) );
	assert ( ! ecs . RemoveComponentChecked <LocationComponent> ( e1 ) );
	assert ( ecs . RemoveComponentChecked <HPComponent> ( e2 ) );
	assert ( ! ecs . GetEntityHasComponent <HPComponent> ( e2 ) );
	ecs . RemoveEntity ( e3 );
	assert ( ! ecs . GetEntityHasComponentChecked <LocationComponent> ( e3 ) );
	std::cout << "(Component creation/deletion) tests passed" << std::endl;
}

void TestECSRegistrations ( ECS & ecs )
{
	TestECSComponentRegistration ( ecs );
	TestECSSystemRegistration ( ecs );
}

void TestECSDataCreation ( ECS & ecs )
{
	TestECSEntityCreation ( ecs );
	TestECSComponentsCreation ( ecs );
}

std::vector <EntityHandle> PrepareECSSystemRun ( ECS & ecs )
{
	auto e1 = ecs . CreateEntity ();
	auto e2 = ecs . CreateEntity ();
	auto e3 = ecs . CreateEntity ();
	auto e4 = ecs . CreateEntity ();

	std::vector <EntityHandle> entityHandles = { e1, e2, e3, e4 };
	std::default_random_engine gen;
	std::uniform_real_distribution <float> distribution ( - 50.f, 50.f );

	for ( std::size_t i = 0; i < entityHandles . size (); i ++ ) {
		EntityHandle e = entityHandles[ i ];
		Vector location = { distribution ( gen ), distribution ( gen ), distribution ( gen ) };
		Vector direction = { distribution ( gen ), distribution ( gen ), distribution ( gen ) };
		float speed = distribution ( gen );

		ecs . AddComponent <MovementComponent> ( e, { e, speed, direction } );
		ecs . AddComponent <LocationComponent> ( e, { e, location } );
		ecs . AddComponent <HPComponent> ( e, { e, 100 } );

		EntityHandle entityToDamage = i % 2 == 1 ? e - 1 : e + 1; // Odds attack even, even attack odds
		ecs . AddComponent <DamageComponent> ( e, { e, entityToDamage, 20 } );
	}
	return entityHandles;
}

void TestECSSystemRun ( ECS & ecs )
{

	PrepareECSSystemRun ( ecs );


	// Initialize initial locations
	std::vector <Vector> initialLocations;
	auto locationComponents = ecs . GetComponentsByType <LocationComponent> () . lock ();
	for ( auto & component : * locationComponents )
		initialLocations . push_back ( component . Location );

	// Run system that moves each component in random direction
	assert ( ecs . RunSystemChecked <MovementSystem> () );


	// Check if each component location was modified
	for ( size_t i = 0; i < locationComponents -> Size (); i ++ ) {
		auto Handle = locationComponents -> GetHandleFromIndex ( i );
		auto & ModifiedComponent = locationComponents -> GetObject ( Handle );
		assert ( ModifiedComponent . Location != initialLocations[ i ] );
	}

	// Initialize initial HP
	std::vector <int> InitialHPArray;
	auto hpComponents = ecs . GetComponentsByType <HPComponent> () . lock ();
	for ( auto & component : * hpComponents )
		InitialHPArray . push_back ( component . HP );

	// Run system that damages each component ( odd entities attack even entities and otherwise )
	assert ( ecs . RunSystemChecked <DamageSystem> () );

	// Check if each component HP was modified.
	for ( size_t i = 0; i < hpComponents -> Size (); i ++ ) {
		auto Handle = hpComponents -> GetHandleFromIndex ( i );
		auto & ModifiedComponent = hpComponents -> GetObject ( Handle );
		assert ( ModifiedComponent . HP != InitialHPArray[ i ] );
	}

	assert ( ! ecs . RunSystemChecked <RenderSystem> () ); // not registered system
	assert ( ecs . RegisterSystemChecked <RenderSystem> () );
	assert ( ecs . RunSystemChecked <RenderSystem> () );
	std::cout << "(Systems run) tests passed" << std::endl;
}

void TestECS ()
{
	ECS ecs;
	TestECSRegistrations ( ecs );
	TestECSDataCreation ( ecs );
	TestECSSystemRun ( ecs );


}

int main ()
{

	std::cout << "------- Starting testing packed array ------" << std::endl;
	TestPackedArray ();
	std::cout << "------ Starting testing Entity Component System ------" << std::endl;
	TestECS ();
	std::cout << "------ All tests passed ------" << std::endl;
	return 0;

}

