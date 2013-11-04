Entitatem
=========

*View the lastest version download information [in the release tab](https://github.com/AndrewBerry/Entitatem/releases).*

Entitatem is a light weight framework that allows developers to easily manage entities and game logic.
Component-entity-systems are an easy and manageable way to break away from old fashioned rigid class hierarchies. Boreal Games has a great explanation of component based entity systems [here](http://www.gamedev.net/page/resources/_/technical/game-programming/understanding-component-entity-systems-r3013).

For an example of how to use Entitatem, see [wiki/ecs-example1](https://github.com/AndrewBerry/Entitatem/wiki/ecs_example1).

Components
=========



BitMasking and Auto-Attaching Systems
=========
When registering a Component type with an Entitatem::Manager, a bitmask is returned (of type Entitatem::uint64).
An entity is represented by and ID and a mask. The entity masks are an indicator of which components are in-use for a particular entity.
With each system that is registered, a required mask value is stored as the minimum requirement that each entity mask has to match before the system is executed.
