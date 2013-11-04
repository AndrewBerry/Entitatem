#ifndef ENTITATEM_MANAGER_HPP
#define ENTITATEM_MANAGER_HPP

#include <vector>
#include <map>

#include <Entitatem/Types.hpp>
#include <Entitatem/System.hpp>
#include <Entitatem/Component.hpp>

#define SystemsMap std::multimap< SystemType, SystemMeta >
#define ComponentMap std::map< const type_info*, ComponentMeta >

namespace Entitatem {
    
    // ------------------------------------------------------------------------
    class Manager {
    public:
                                    Manager( const uint32& a_entityMaxCount );

        void                        ExecuteUpdateSystems();
        void                        ExecuteRenderSystems();

        // entities
        uint32                      GetUnusedEntity();
		uint32                      GetLastUnusedEntity();
        inline const uint32&        GetEntityMaxCount() { return m_entityMaxCount; };
        inline void                 SetEntityMask( const uint32& a_id, const uint64& a_mask ) { m_entityMasks[ a_id ] = a_mask; };
        inline const uint64&        GetEntityMask( const uint32& a_id ) { return m_entityMasks[ a_id ]; };

        // systems
        void                        RegisterSystem( const SystemType& a_type, System* a_system );

        // components
        template < class T >
        const uint64&               RegisterComponent();
        template < class T >
        uint64                      GetComponentMask();
        template < class T >
        T*                          GetComponentData( const uint32& a_id );

    private:
        // entities
        const uint32                m_entityMaxCount;
        std::unique_ptr< uint64[] > m_entityMasks;

        // systems
        SystemsMap                  m_systems;
        SystemsMap::iterator        m_updateSystems, m_renderSystems;

        // components
        ComponentMap                m_components;

    };

};

#include "Manager.inl"

#endif
