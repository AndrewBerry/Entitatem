#include <Entitatem/Entitatem.hpp>
#include <iostream>

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
    void System::SetRequirement( const std::string& a_key, const Pattern& a_patternType ) {
        m_patternType = a_patternType;

        std::istringstream keyStr( a_key );
        std::string key;
        while ( !keyStr.eof() ) {
            keyStr >> key;
            size_t hash = std::hash< std::string >()( key );
            m_requirements.push_back( hash );
        };
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
        std::istringstream keyStr( a_key );
        std::string key;
        while ( !keyStr.eof() ) {
            keyStr >> key;
            size_t hash = std::hash< std::string >()( key );

            auto iter = m_components.find( hash );
            if ( iter == m_components.end() ) {
                throw "Could not set entity flag - Component does not exist.";
            };

            m_entityMasks[ a_id ][ iter->second.m_maskId ] = 1;
        };
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
                std::bitset< MAX_COMPONENTS > systemMask = GenerateSystemMask( system->GetRequirements() );

                for ( auto ei = m_entitiesInUse.begin(); ei != m_entitiesInUse.end(); ++ei ) {
                    if ( MasksMatch( system->GetPatternType(), systemMask, m_entityMasks[ *ei ] ) ) {
                        system->Execute( *ei );
                    };
                };
            } else {
                system->Execute();
            };
        };
    };
    
    // ------------------------------------------------------------------------
    std::bitset< MAX_COMPONENTS > Manager::GenerateSystemMask( const std::vector< size_t >& a_hash ) const {
        std::bitset< MAX_COMPONENTS > resultBits;

        for ( auto hit = a_hash.begin(); hit != a_hash.end(); ++hit ) {
            auto result = m_components.find( *hit );

            if ( result != m_components.end() ) {
                resultBits[ result->second.m_maskId ] = 1;
            } else {
                throw "Could not generate system mask - Component does not exist.";
            };
        };

        return resultBits;
    };
    
    // ------------------------------------------------------------------------
    bool Manager::MasksMatch( const System::Pattern& a_patternType, const std::bitset< MAX_COMPONENTS >& a_sys, const std::bitset< MAX_COMPONENTS >& a_ent ) const {
        const std::bitset< MAX_COMPONENTS >& rhs = ( a_patternType == System::MIN ) ? a_sys : a_ent;
        return ( a_sys & a_ent ) == rhs;
    };

};
