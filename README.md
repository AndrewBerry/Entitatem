Entitatem
=========

*View the lastest version download information [in the release tab](/releases).*

Entitatem is a light weight framework that allows developers to easily manage entities and game logic.
Component-entity-systems are an easy and manageable way to break away from old fashioned rigid class hierarchies. Boreal Games has a great explanation of component based entity systems [here](http://www.gamedev.net/page/resources/_/technical/game-programming/understanding-component-entity-systems-r3013).

>Entities are your game objects, which are implicitly defined by a collection of components. These components are pure data and are operated on in functional groups by the systems.

System Types
=========
Systems are split up into two types, Global Systems and Entity Systems. The system type is determined by the Component Requirements - Systems with ANY requirements are classified as Entity Systems and systems without requirements are classified as Global Systems.

System Type | Function Called
--- | ---
Global System | `System::Execute()`
Entity System | `System::Execute( const size_t& a_id )`*

*`a_id` is the ID of the entity the system is being executed on.

The system delay determines wether the system is an update system or a render system. Any system with a delay greater than 0 are Update systems and all systems with a delay of 0 are render systems.
