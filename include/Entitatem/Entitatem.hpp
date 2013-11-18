#ifndef ENTITATEM_HPP
#define ENTITATEM_HPP

#include <functional>
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <set>

#define MAX_COMPONENTS 64u

namespace Entitatem {
    
    // ------------------------------------------------------------------------
    class System {
    public:
                                                System( const size_t& a_delay );

        virtual void                            Execute();
        virtual void                            Execute( const size_t& a_id );

        void                                    AddRequirement( const std::string& a_key );

    protected:
        friend class Manager;
        inline const std::vector< size_t >&     GetRequirements() const { return m_requirements; };
        inline const size_t&                    GetDelay() const { return m_delay; };
        size_t&                                 GetFrameSkip() { return m_frameSkip; };

    private:
        std::vector< size_t >                   m_requirements;
        size_t                                  m_delay, m_frameSkip;

    };

    // ------------------------------------------------------------------------
    class Manager {
    public:
                                                Manager( const size_t& a_maxEntities );
                                                ~Manager();
        const size_t                            MAX_ENTITIES;
        
        inline void                             Update() { Execute( m_systemsUpdate ); };
        inline void                             Render() { Execute( m_systemsRender ); };

        // Entities
        size_t                                  GetNewEntity();
        void                                    DestroyEntity( const size_t& a_id );
        void                                    SetEntityFlag( const std::string& a_key, const size_t& a_id, const bool& a_value );
        
        // Components
        template < class T >
        const size_t&                           RegisterComponent( const std::string& a_key );
        template < class T >
        T&                                      Get( const std::string& a_key, const size_t& a_id );

        // Systems
        void                                    RegisterSystem( System* a_system );

    private:
        // Entities
        std::vector< std::bitset< MAX_COMPONENTS > > m_entityMasks;
        std::set< size_t >                      m_entitiesFree, m_entitiesInUse;

        // Components
        struct CompMeta {
            size_t                              m_typeSize, m_maskId;
            char*                               m_memBlock;
        };
        std::map< size_t, CompMeta >            m_components;

        // Systems
        std::vector< System* >                  m_systemsUpdate, m_systemsRender;
        void                                    Execute( std::vector< System* >& a_systems );

    };
};

#include "Entitatem.inl"
#endif
