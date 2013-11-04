#include <Entitatem/Manager.hpp>

namespace Entitatem {
    
    // ------------------------------------------------------------------------
    Manager::Manager( const uint32& a_entityMaxCount ) :
        m_entityMaxCount( a_entityMaxCount )
    {
        m_entityMasks = std::unique_ptr< uint64[] >( new uint64[ a_entityMaxCount ] );

        for ( uint32 id = 0; id < m_entityMaxCount; ++id ) {
            SetEntityMask( id, 0u );
        };
    };
    
    // ------------------------------------------------------------------------
    void Manager::ExecuteUpdateSystems() {
        for ( auto system = m_updateSystems; system != m_renderSystems; ++system ) {
            system->second.m_skippedFrames++;

            if ( system->second.m_skippedFrames >= system->second.m_system->GetDelay() ) {
                if ( system->second.m_system->GetMask() > 0u ) {
                    for ( uint32 id = 0u; id < m_entityMaxCount; ++id ) {
                        if ( ( GetEntityMask( id ) & system->second.m_system->GetMask() ) == system->second.m_system->GetMask() ) {
                            system->second.m_system->Execute( id );
                        };
                    };
                } else {
                    system->second.m_system->Execute();
                };

                system->second.m_skippedFrames = 0u;
            };
        };
    };
    
    // ------------------------------------------------------------------------
    void Manager::ExecuteRenderSystems() {
        for ( auto system = m_renderSystems; system != m_systems.end(); ++system ) {
            system->second.m_skippedFrames++;

            if ( system->second.m_skippedFrames >= system->second.m_system->GetDelay() ) {
                if ( system->second.m_system->GetMask() > 0u ) {
                    for ( uint32 id = 0u; id < m_entityMaxCount; ++id ) {
                        if ( ( GetEntityMask( id ) & system->second.m_system->GetMask() ) == system->second.m_system->GetMask() ) {
                            system->second.m_system->Execute( id );
                        };
                    };
                } else {
                    system->second.m_system->Execute();
                };

                system->second.m_skippedFrames = 0u;
            };
        };
    };
    
    // ------------------------------------------------------------------------
    uint32 Manager::GetUnusedEntity() {
        for ( uint32 id = 0u; id < m_entityMaxCount; ++id ) {
            if ( GetEntityMask( id ) == 0u ) {
                return id;
            };
        };

        return m_entityMaxCount;
    };

    // ------------------------------------------------------------------------
    void Manager::RegisterSystem( const SystemType& a_type, System* a_system ) {
        SystemMeta metaData;
        metaData.m_system = std::shared_ptr< System >( a_system );
        metaData.m_skippedFrames = 0u;

        m_systems.insert( std::pair< SystemType, SystemMeta >( a_type, metaData ) );
        m_updateSystems = m_systems.find( SYSTEM_UPDATE );
        m_renderSystems = m_systems.find( SYSTEM_RENDER );
    };

};
