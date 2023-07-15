#include <iostream>
#include "Entity.h"
#include "EntityAction.h"
#include "EntityActionManager.h"
#include <cassert>


int main()

{

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


    return 0;

}
