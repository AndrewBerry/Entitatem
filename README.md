# Entitatem
*View the lastest version download information [in the release tab](https://github.com/AndrewBerry/Entitatem/releases).*

Entitatem is a light weight framework that allows developers to easily manage entities and game logic.
Component-entity-systems are an easy and manageable way to break away from old fashioned rigid class hierarchies. Boreal Games has a great explanation of component-entity-systems [here](http://www.gamedev.net/page/resources/_/technical/game-programming/understanding-component-entity-systems-r3013).

>Entities are your game objects, which are implicitly defined by a collection of components. These components are pure data and are operated on in functional groups by the systems.

## Quick Tutorial
*Please note that any code shown here should be considered as psuedo-code.*
### Setting up Entitatem in a game container and game loop
Below is a demonstration of how to set up Entitatem to use within a traditional game loop.

````
Entitatem::Manager manager( 64u );

manager.RegisterComponent< float >( "rotation" );
manager.RegisterComponent< float >( "speed" );

manager.RegisterSystem( &MovementSystem() );

while ( true ) {
  manager.Update();
  manager.Render();
};
````

This is all you will need to get the entity manager running with a system and components. No entities exist in the world yet.


### Defining a component
ECS components by definition are purely data but Entitatem supports custom type components with helper methods (mutators, accessors, etc). Multiple components of the same type can be registered as long as their identifier is unique.

````
class PositionComponent {
public:
    float x, y;
    void Set( a_x, a_y );
};

manager.RegisterComponent< PositionComponent >( "position" );
````

Primitive built-in data types are also supported as components as seen above in the game loop example code.

### Creating an entity
Entities are represented by their id which is used to locate their data in memory.

````
size_t entity = manager.GetNewEntity();
if ( entity < manager.MAX_ENTITIES ) {
  manager.SetEntityMask( "rotation speed", entity, true );
  
  manager.Get< float >( "rotation", entity ) = 0.0f;
  manager.Get< float >( "speed", entity ) = 0.0f;
};
````
The call to `manager.GetNewEntity()` returns an unused entity identifier. If no entities are available, `manager.MAX_ENTITIES` is returned. `manager.SetEntityMask` tells the manager that the entity is using a rotation component and a speed component - which will be used to filter which systems should be executed on this entity. `Get` returns a reference to the component so the data can also be set.

### System types
Systems are split up into two types, Global Systems and Entity Systems. The system type is determined by the Component Requirements - Systems with ANY requirements are classified as Entity Systems and systems without requirements are classified as Global Systems.

System Type | Function Called
--- | ---
Global System | `System::Execute()`
Entity System | `System::Execute( const size_t& a_id )`*

*`a_id` is the ID of the entity the system is being executed on.

The system delay determines wether the system is an update system or a render system. Any system with a delay greater than 0 are Update systems and all systems with a delay of 0 are render systems.

### Defining an entity system
When constructing an entity system, `Entitatem::System::SetRequirements( "..." )` is used to compare with an entity mask and determine if `System::Execute( const size_t& a_id )` will be called or not.

````
class MovementSystem {
public:
    MovementSystem() : Entitatem::System( 1u )
    {
        SetRequirement( "rotation speed" );
    };
    
    void Execute( const size_t& a_id ) {
        /* --- Move/rotate entity a_id --- */
    };
};
````
This system will be automatically executed on any entity which contains both a rotation component and a speed component. By default the filter uses a minimum requirement (any entity with AT LEAST a rotataion and speed component will be executed on), this however can be changed to a maximum requirement (any entity with ONLY a rotation and speed component will be executed on) by setting the second paramater in `SetRequirement` to `Entitatem::System::MAX`.

Eg.
````
class MovementSystem {
...
    MovementSystem() : Entitatem::System( 1u )
    {
        SetRequirement( "rotation speed", Entitatem::System::MAX );
    };
...
};
````

### Defining a global system
Global systems are defined the same way as entity systems only `SetRequirement()` is not necessary (this is what qualifies a system as an entity system) as the system calls the overloaded `Execute()` method one per frame, reguardless of entity masks.
