#ifndef ENTITATEM_SYSTEM_HPP
#define ENTITATEM_SYSTEM_HPP

#include <memory>
#include <string>

#include <Entitatem/Types.hpp>

namespace Entitatem {

    // ------------------------------------------------------------------------
    enum SystemType {
        SYSTEM_UPDATE,
        SYSTEM_RENDER
    };
    
    // ------------------------------------------------------------------------
    class System {
    public:
                                    System( const uint64& a_mask, const uint32& a_delay = 0u );
                                    
        virtual void                Execute();
        virtual void                Execute( const uint32& a_id );

        inline const uint64&        GetMask() const { return m_mask; };
        inline const uint32&        GetDelay() const { return m_delay; };

    private:
        uint64                        m_mask;
        uint32                        m_delay;

    };
    
    // ------------------------------------------------------------------------
    struct SystemMeta {
        std::shared_ptr< System >    m_system;
        uint32                        m_skippedFrames;

    };

};

#endif
