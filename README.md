Entitatem
=========

*View the lastest version download information [in the release tab](https://github.com/AndrewBerry/Entitatem/releases).*

Entitatem is a light weight framework that allows developers to easily manage entities and game logic.
Component-entity-systems are an easy and manageable way to break away from old fashioned rigid class hierarchies. Boreal Games has a great explanation of component based entity systems [here](http://www.gamedev.net/page/resources/_/technical/game-programming/understanding-component-entity-systems-r3013).

For an example of how to use Entitatem, see [wiki/ecs-example1](https://github.com/AndrewBerry/Entitatem/wiki/ecs_example1).

Components, Systems and Masking
=========
Component masks are generated when a component is registered with an Entitatem::Manager.

```
// eg.
Entitatem::uint64 COMP1_MASK = manager.RegisterComponent< Component1 >(); // will return 1 [0..001]
Entitatem::uint64 COMP2_MASK = manager.RegisterComponent< Component2 >(); // will return 2 [0..010]
Entitatem::uint64 COMP3_MASK = manager.RegisterComponent< Component3 >(); // will return 3 [0..100]
```
These component masks are used to signify which components are in-use for an entity and the requirement mask for each system.

```
// eg.
manager.SetEntityMask( 0u, COMP1_MASK | COMP3_MASK ); // sets entity 0's mask to 5 [0..101].
```
This means any system that requires a Component1 and Component2 will automatically be executed on this entity.

