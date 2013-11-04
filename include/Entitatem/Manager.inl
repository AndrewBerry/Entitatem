#include <Entitatem/Manager.hpp>

namespace Entitatem {
    
    // ------------------------------------------------------------------------
    template < class T >
    const uint64& Manager::RegisterComponent() {
        ComponentMeta metaData;
        metaData.m_data = std::shared_ptr< Component >( new T[ m_entityMaxCount ], std::default_delete<T[]>() );
        metaData.m_mask = ( uint64 ) 1 << ( uint64 ) m_components.size();
        
        m_components.insert( std::make_pair< const type_info*, ComponentMeta >( &typeid( T ), metaData ) );

        return metaData.m_mask;
    };
    
    // ------------------------------------------------------------------------
    template < class T >
    uint64 Manager::GetComponentMask() {
        auto result = m_components.find( &typeid( T ) );
        if ( result != m_components.end() ) {
            return result->second.m_mask;
        };

        return 0u;
    };

    // ------------------------------------------------------------------------
    template < class T >
    T* Manager::GetComponentData( const uint32& a_id ) {
        auto result = m_components.find( &typeid( T ) );
        if ( result != m_components.end() ) {
            auto& component = result->second;
            
            if ( component.m_mask & GetEntityMask( a_id ) ) {
                std::shared_ptr< T > data = std::dynamic_pointer_cast< T >( component.m_data );
                return &(data.get()[ a_id ]);
            };
        };

        return nullptr;
    };

};
