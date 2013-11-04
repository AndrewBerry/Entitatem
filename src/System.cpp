#include <Entitatem/System.hpp>

namespace Entitatem {

    // ------------------------------------------------------------------------
    System::System( const uint64& a_mask, const uint32& a_delay ) :
        m_mask( a_mask ),
        m_delay( a_delay )
    {
        // --
    };
    
    // ------------------------------------------------------------------------
    void System::Execute() {
        // --
    };
    
    // ------------------------------------------------------------------------
    void System::Execute( const uint32& a_id ) {
        // --
    };

};
