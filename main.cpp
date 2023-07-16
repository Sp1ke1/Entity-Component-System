#include <iostream>
/*#include "ecs/Entity.h"
#include "EntityAction.h"
#include "EntityActionManager.h"*/
#include "PackedArray.h"
#include <cassert>


void TestMappedVector()
{
    PackedArray <int> data;

    int var;

    auto id1 = data.Add( 3 );
    auto id2 = data.Add( 4 );
    auto id3 = data.Add ( 10 );

    assert ( data.Get ( id3, var ) && var == 10 );

    assert ( data.Get ( id2, var  ) && var == 4 );

    data.Remove ( id2 );

    assert ( data.Get( id1, var ) && var == 3 );
    assert ( data.Get ( id3, var ) && var == 10 );

    auto id4 = data.Add( 25 );

    assert ( data.Get ( id1, var ) && var == 3 );
    assert ( data.Get ( id3, var ) && var == 10 );
    assert ( data.Get ( id4, var ) && var == 25 );

    data.Clear();

    assert ( !data.Get ( id1, var ) );
    assert ( !data.Get( id3, var ) );

    auto id5 = data.Add( 8 );
    assert ( data.Get ( id5, var ) && var == 8 );
    data.Remove( id5 );
    assert ( !data.Get( id5, var ) );

}

int main()

{

    TestMappedVector();
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
