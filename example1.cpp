#include <iostream>
#include <Entitatem/Entitatem.hpp>

// ----------------------------------------------------------------------------
class GlobalRenderSystem : public Entitatem::System {
public:
    GlobalRenderSystem() : Entitatem::System( 0u ) {};
    void Execute() { std::cout << "GlobalRenderSystem::Execute()\n"; };

};

// ----------------------------------------------------------------------------
class EntityRenderSystem : public Entitatem::System { 
public:
    EntityRenderSystem() : Entitatem::System( 0u ) { SetRequirement( "health" ); };
    void Execute( const size_t& a_id ) { std::cout << "EntityRenderSystem::Execute( " << a_id << " )\n"; };

};

// ----------------------------------------------------------------------------
class GlobalUpdateSystem : public Entitatem::System {
public:
    GlobalUpdateSystem() : Entitatem::System( 1u ) {};
    void Execute() { std::cout << "GlobalUpdateSystem::Execute()\n"; };

};

// ----------------------------------------------------------------------------
class EntityUpdateSystem : public Entitatem::System {
public:
    EntityUpdateSystem() : Entitatem::System( 1u ) { SetRequirement( "health level" ); };
    void Execute( const size_t& a_id ) { std::cout << "EntityUpdateSystem::Execute( " << a_id << " )\n"; };

};

// ----------------------------------------------------------------------------
bool RegisterComponents( Entitatem::Manager& a_manager );
size_t CreateEntity( Entitatem::Manager& a_manager );

// ----------------------------------------------------------------------------
int main() {
    Entitatem::Manager m( 64u );
    if ( !RegisterComponents( m ) ) {
        return 1;
    };

    GlobalRenderSystem grs;
    EntityRenderSystem ers;
    GlobalUpdateSystem gus;
    EntityUpdateSystem eus;

    m.RegisterSystem( &grs );
    m.RegisterSystem( &ers );
    m.RegisterSystem( &gus );
    m.RegisterSystem( &eus );
    
    CreateEntity( m );
    CreateEntity( m );

    for ( size_t i = 0u; i < 5u; ++i ) {
        m.Update();
        m.Render();
    };

    std::cin.get();
};

// ----------------------------------------------------------------------------
bool RegisterComponents( Entitatem::Manager& a_manager ) {
    try {

        a_manager.RegisterComponent< float >( "health" );
        a_manager.RegisterComponent< int >( "level" );

    } catch( char* exc ) {
        std::cout << exc << '\n';
        return false;
    };

    return true;
};

// ----------------------------------------------------------------------------
size_t CreateEntity( Entitatem::Manager& a_manager ) {
    size_t id = a_manager.GetNewEntity();

    if ( id < a_manager.MAX_ENTITIES ) {
        try {
            a_manager.SetEntityMask( "health level", id, true );
            
            a_manager.Get< float >( "health", id ) = 100.0f;
            a_manager.Get< int >( "level", id ) = 3;

            return id;
        } catch ( char* exc ) {
            std::cout << exc << '\n';
            return a_manager.MAX_ENTITIES;
        };
    };

    return a_manager.MAX_ENTITIES;
};
