#include <iostream>
#include "ECSManager.h"
#include "ExampleComponents.h"
#include <cassert>


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

void TestECS()
{
    ECSManager ecs;

    auto e1 = ecs.CreateEntity();
    auto e2 = ecs.CreateEntity();

    assert ( ecs.RegisterComponent<LocationComponent>());
    assert ( ecs.RegisterComponent<MovementComponent>());
    assert ( ecs.RegisterComponent<HPComponent>());

    Vector loc1 { 1.0f, 1.0f, 0.0f };
    Vector loc2 { 10.0f, 10.0f, -10.f };
    Vector loc3 { 5.0f, -5.0f, 10.0f };
    assert ( ecs.AddComponent ( e1, LocationComponent ( { 0.0f, 1.0f, 2.0f } )));

    auto & Comp = ecs.GetComponent<LocationComponent> ( e1 );
    Comp.Location.X = 10.f;
    auto & CompUpd = ecs.GetComponent<LocationComponent>( e1 );
    assert ( CompUpd.Location.X  == 10.f );

    assert ( ecs.RemoveEntity( e2 ) );
    auto e3 = ecs.CreateEntity();

    ecs.AddComponent <MovementComponent> ( e3 ,{ 10.f, { 0.0f, 1.0f, 0.0f } } );
    ecs.AddComponent <MovementComponent> ( e1, { 0.5f, { 0.0f, 1.0f, 0.0f } } );

    auto comp1 = ecs.GetComponent<MovementComponent>( e3 );
    auto comp2 = ecs.GetComponent<MovementComponent>( e1 );

    assert ( comp1.Speed == 10.f && comp2.Speed == 0.5f );

}

int main()

{

    TestPackedArray();
    TestECS ();

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
