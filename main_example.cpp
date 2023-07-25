#include "ECS.h"
#include "Example/EntityActionSystemSwitcher.h"
#include "Example/EntityActionSystem.h"
#include "Example/PrivateEntityAction.h"
#include "Example/PublicEntityAction.h"
#include "Example/EntityActionSystemComponentRemover.h"

void RegisterTypes ( ECS & ecs )
{
	std::cout << "--- Registering ECS types" << std::endl;
	ecs . RegisterComponent <EntityActionManagerComponent> ();
	std::cout << "Registered EntityActionManagerComponent" << std::endl;
	ecs . RegisterSystem <EntityActionSystem, EntityActionManagerComponent> ();
	std::cout << "Registered EntityActionSystem" << std::endl;
	ecs . RegisterSystem <EntityActionSystemSwitcher, EntityActionManagerComponent> ();
	std::cout << "Registered EntityActionSystemSwitcher" << std::endl;
	ecs . RegisterSystem <EntityActionSystemComponentRemover, EntityActionManagerComponent> ();
	std::cout << "Registered EntityActionSystemComponentRemover" << std::endl;
}


void FillEcsData ( ECS & ecs )
{
	std::cout << "--- Filling ECS with data" << std::endl;

	auto e1 = ecs . CreateEntity();
	auto e2 = ecs . CreateEntity();

	EntityActionManagerComponent managerComponent1;
	EntityActionManagerComponent managerComponent2;

	managerComponent1 . AddObject ( std::make_shared <PrivateEntityAction> ( e1, 0 ) );
	managerComponent1 . AddObject ( std::make_shared <PrivateEntityAction> ( e1, 1 ) );
	managerComponent1 . AddObject ( std::make_shared <PublicEntityAction> ( e1, 2 ) );

	managerComponent2 . AddObject ( std::make_shared <PrivateEntityAction> ( e2, 3 ) );
	managerComponent2 . AddObject ( std::make_shared <PublicEntityAction> ( e2, 4 ) );

	ecs . AddComponent <EntityActionManagerComponent> ( e1, managerComponent1 );
	ecs . AddComponent <EntityActionManagerComponent> ( e2, managerComponent2 );

	std::cout << "Created 2 entities with EntityActionManager components. Sets of entity actions 1:{Private Private Public} 2:{Private Public}" << std::endl;
}

void RunSystems ( ECS & ecs )
{
	std::cout << "--- Running systems" << std::endl;
	std::cout << "- Running EntityActionSystem" << std::endl;
	ecs . RunSystem <EntityActionSystem> ();
	std::cout << "Running EntityActionSystemSwitch (switches entity actions visibility)" << std::endl;
	ecs . RunSystem <EntityActionSystemSwitcher> ();
	std::cout << "- Running EntityActionSystem after switching visibility" << std::endl;
	ecs . RunSystem <EntityActionSystem> ();
	std::cout << "Running EntityActionSystemComponentRemover (will remove all private actions)" << std::endl;
	ecs . RunSystem <EntityActionSystemComponentRemover> ();
	std::cout << "- Running EntityActionSystem after removing all private actions" << std::endl;
	ecs . RunSystem <EntityActionSystem> ();
	std::cout << "Running EntityActionSystemSwitch again (switches entity actions visibility)" << std::endl;
	ecs . RunSystem <EntityActionSystemSwitcher> ();
	std::cout << "- Running EntityActionSystem" << std::endl;
	ecs . RunSystem <EntityActionSystem> ();

	return;
}

int main ()
{
	std::cout << "------ Entity Component System example ------" << std::endl;

	ECS ecs;
	RegisterTypes ( ecs );
	FillEcsData ( ecs );
	RunSystems ( ecs );

	std::cout << "------ Exiting ------" << std::endl;

	return 0;
}