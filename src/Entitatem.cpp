#include <Entitatem/Entitatem.hpp>

namespace Entitatem {

    // ------------------------------------------------------------------------
    System::System( const size_t& a_delay ) :
        m_frameSkip( 0u ),
        m_delay( a_delay )
    {
        /* --- --- */
    };

    // ------------------------------------------------------------------------
    void System::Execute() {
        /* --- --- */
    };

    // ------------------------------------------------------------------------
    void System::Execute( const size_t& a_id ) {
        /* --- --- */
    };
    
    // ------------------------------------------------------------------------
    void System::AddRequirement( const std::string& a_key ) {
        size_t hash = std::hash< std::string >()( a_key );
        m_requirements.push_back( hash );
    };

    // ------------------------------------------------------------------------
    Manager::Manager( const size_t& a_maxEntities ) :
        MAX_ENTITIES( a_maxEntities ),
        m_entityMasks( a_maxEntities )
    {
        for ( size_t i = 0u; i < MAX_ENTITIES; ++i ) {
            m_entitiesFree.insert( i );
        };
    };
        
    // ------------------------------------------------------------------------
    Manager::~Manager() {
        for ( auto compMeta = m_components.begin(); compMeta != m_components.end(); ++compMeta ) {
            delete[] compMeta->second.m_memBlock;
        };
    };
    
    // ------------------------------------------------------------------------
    size_t Manager::GetNewEntity() {
        if ( m_entitiesFree.size() > 0u ) {
            size_t id = *m_entitiesFree.begin();
            m_entitiesFree.erase( m_entitiesFree.begin() );
            m_entitiesInUse.insert( id );

            return id;
        };

        return MAX_ENTITIES;
    };

    // ------------------------------------------------------------------------
    void Manager::DestroyEntity( const size_t& a_id ) {
        if ( m_entityMasks[ a_id ].any() ) {
            m_entityMasks[ a_id ].reset();
            m_entitiesInUse.erase( a_id );
            m_entitiesFree.insert( a_id );
        };
    };

    // ------------------------------------------------------------------------
    void Manager::SetEntityFlag( const std::string& a_key, const size_t& a_id, const bool& a_value ) {
        size_t hash = std::hash< std::string >()( a_key );

        auto iter = m_components.find( hash );
        if ( iter == m_components.end() ) {
            throw "Could not set entity flag - Component does not exist.";
        };

        m_entityMasks[ a_id ][ iter->second.m_maskId ] = 1;
    };
    
    // ------------------------------------------------------------------------
    void Manager::RegisterSystem( System* a_system ) {
        std::vector< System* >& systems = ( a_system->GetDelay() == 0u ) ? m_systemsRender : m_systemsUpdate;
        systems.push_back( a_system );
    };
    
    // ------------------------------------------------------------------------
    void Manager::Execute( std::vector< System* >& a_systems ) {
        for ( auto si = a_systems.begin(); si != a_systems.end(); ++si ) {
            System* system = *si;

            if ( system->GetDelay() > 0u ) {
                system->GetFrameSkip()++;

                if ( system->GetFrameSkip() < system->GetDelay() ) {
                    continue;
                };
            };

            system->GetFrameSkip() = 0u;
            if ( system->GetRequirements().size() > 0u ) {
                for ( auto ei = m_entitiesInUse.begin(); ei != m_entitiesInUse.end(); ++ei ) {
                    system->Execute( *ei );
                };
            } else {
                system->Execute();
            };
        };
    };

};
