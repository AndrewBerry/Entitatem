namespace Entitatem {

    // ------------------------------------------------------------------------
    template < class T >
    const size_t& Manager::RegisterComponent( const std::string& a_key ) {
        if ( m_components.size() == MAX_COMPONENTS ) {
            throw "Could not allocate component - Attempting to create more than `MAX_COMPONENTS` components.";
        };

        size_t hash = std::hash< std::string >()( a_key );
        if ( m_components.find( hash ) != m_components.end() ) {
            throw "Could not allocate component - Duplicate key detected.";
        };

        CompMeta compMeta;
        compMeta.m_typeSize = sizeof( T );
        compMeta.m_maskId = m_components.size();
        compMeta.m_memBlock = new char[ compMeta.m_typeSize * MAX_ENTITIES ];

        m_components[ hash ] = compMeta;
        return compMeta.m_maskId;
    };

    // ------------------------------------------------------------------------
    template < class T >
    T& Manager::Get( const std::string& a_key, const size_t& a_id ) {
        size_t hash = std::hash< std::string >()( a_key );

        auto iter = m_components.find( hash );
        if ( iter == m_components.end() ) {
            throw "Could not return component - Component does not exist.";
        };

        CompMeta& compMeta = iter->second;
        if ( compMeta.m_typeSize != sizeof( T ) ) {
            throw "Could not return component - Memory block allignment is incorrect.";
        };

        if ( !m_entityMasks[ a_id ].test( compMeta.m_maskId ) ) {
            throw "Could not return component - Entity mask does not match.";
        };

        return *reinterpret_cast< T* >( compMeta.m_memBlock + ( sizeof( T ) * a_id ) );
    };

};
