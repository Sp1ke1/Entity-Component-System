#include <iostream>
#include "ECS.h"
#include "Test/TestSystems.h"
#include "Test/TestComponents.h"
#include <cassert>
#include <chrono>
#include <random>


void TestPackedArray ()
{
	PackedArray <int> data;

	auto id1 = data.Add( 3 );
	auto id2 = data.Add( 4 );
	auto id3 = data.Add ( 10 );

	assert ( data.Get( id1 ) == 3 );
	assert ( data.Get ( id2 ) == 4 );
	assert ( data.Get ( id3 ) == 10 );

	data.RemoveChecked ( id2 );

	assert ( data.Get ( id1 ) == 3 );
	assert ( data.Get ( id3 ) == 10 );
	assert ( !data.GetChecked ( id2 ));

	auto id4 = data.Add( 25 );

	assert (data.Get(id1) == 3 );
	assert (data.Get(id3) == 10 );
	assert (data.Get(id4) == 25 );

	data.Clear();

	assert ( !data.GetChecked(id1) );
	assert ( !data.GetChecked(id3) );

	auto id5 = data.Add( 8 );
	assert ( data.Get ( id5 ) == 8 );

	assert ( data.RemoveChecked(id5) );
	assert ( !data.GetChecked(id5 ) );

}
void TestECSRegistrations ( ECS & ecs )
{
    ecs.RegisterComponentChecked<LocationComponent>();
	assert ( ecs. GetIsComponentRegistered <LocationComponent>() );
    assert ( ecs.RegisterComponentChecked<MovementComponent>());
    assert ( ecs.RegisterComponentChecked<DamageComponent>());
    assert ( ecs.RegisterComponentChecked<HPComponent>());
    assert ( !ecs.RegisterComponentChecked<HPComponent>() );
	assert ( ecs. GetIsComponentRegistered <HPComponent> () );
	std::cout << "Component registration tests passed" << std::endl;

	
	ecs . RegisterSystem <MovementSystem, LocationComponent, MovementComponent> ();
	assert ( ecs . GetIsSystemRegistered <MovementSystem>() );
	assert ( !ecs . GetIsSystemRegistered <DamageSystem>() );
	bool Registered = ecs . RegisterSystemChecked <DamageSystem, DamageComponent, HPComponent> ();
	assert ( Registered );
	Registered = ecs . RegisterSystemChecked <DamageSystem, HPComponent, DamageComponent>();
	assert ( !Registered );
	assert ( ecs . GetIsSystemRegistered <DamageSystem>() );
	std::cout << "System registration tests passed" << std::endl;

}

void TestECSDataFill ( ECS & ecs, int nOfEntities )
{

	std::default_random_engine gen;
	std::uniform_real_distribution<float> distribution  ( -50.f, 50.f );
	std::uniform_int_distribution<int> int_distributaion ( 1, 1000 );

	std::vector <EntityHandle> handles;
	for ( int i = 0; i < nOfEntities; i ++ )
	{
		auto entity = ecs.CreateEntity();
		auto location = Vector { distribution ( gen ), distribution ( gen ), distribution ( gen ) };
		auto speed = distribution ( gen );
		/*
		assert ( ECS.AddComponent ( entity, LocationComponent ( entity, location )));
		assert ( ECS.AddComponent( entity, MovementComponent ( entity, speed, location )));
		assert ( ECS.AddComponent( entity, HPComponent ( entity, 100 ) ) );
*/

		ecs.AddComponent ( entity, LocationComponent ( entity, location ));
		ecs.AddComponent( entity, MovementComponent ( entity, speed, location ));
		ecs.AddComponent( entity, HPComponent ( entity, int_distributaion ( gen ) ) ) ;


		handles.push_back ( entity );
	}

	for ( auto e : handles )
	{
		if ( e % 2 == 0 && e > 0 )
		{
			/*assert ( ECS.AddComponent( e, DamageComponent { e, e-1, int_distributaion ( gen ) } ) )*/;
			ecs.AddComponent( e, DamageComponent { e, e-1, int_distributaion ( gen ) } );
		}
	}
}


void TestECS( int nOfEntities )
{
	ECS ecs;
	TestECSRegistrations( ecs );
	TestECSDataFill( ecs, nOfEntities );

	std::cout << "Running profiling on : " << nOfEntities << " entities" << std::endl;
	std::cout << "////////////////////////////////////////////////////////////////" << std::endl;

	for ( int i = 0; i < 100; i ++ )
	{
		std::cout << " ------------- " << std::endl;
		auto start = std::chrono::system_clock::now();
		ecs.RunSystem<DamageSystem>();
		ecs.RunSystem<MovementSystem>();
		auto end = std::chrono::system_clock::now();
		std::cout << "Tick. Elapsed time: " << std::chrono::duration_cast<std::chrono::milliseconds> ( end - start ).count() << "ms" << std::endl;
	}
	std::cout << "////////////////////////////////////////////////////////////////" << std::endl;






}

int main( int argc, char ** argv )

{
	int nOfEntities = 10;

	if ( argc > 1 )
	{
		nOfEntities = std::stoi ( std::string ( argv[1] ) );
	}
	TestPackedArray();
	TestECS ( nOfEntities );


/*

    Entity<EntityAction> e1 ( 0 );

    EntityAction ea1 ( &e1 );
    EntityAction ea2 ( &e1 );
    EntityAction ea3 ( &e1 );
    EntityAction ea4 ( &e1 );


    ea1.SetIsVisible( true );
    ea2.SetIsVisible( false );
    ea3.SetIsVisible( true );
    ea4.SetIsVisible( true );

    auto & ObjectManager = static_cast <EntityActionManager&> ( e1.GetObjectManager() );
    auto VisibleObjects = ObjectManager . GetAllVisibleActions();
    assert ( VisibleObjects.size() == 3 );

    ea1.OnRemoved();
    VisibleObjects = ObjectManager . GetAllVisibleActions();
    assert ( VisibleObjects.size() == 2 );
    ea3.SetIsVisible( false );
    VisibleObjects = ObjectManager . GetAllVisibleActions();
    assert ( VisibleObjects.size() == 1 );
*/


	return 0;

}
