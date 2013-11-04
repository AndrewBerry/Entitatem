#ifndef ENTITATEM_COMPONENT_HPP
#define ENTITATEM_COMPONENT_HPP

#include <memory>

#include <Entitatem/Types.hpp>

namespace Entitatem {
    
    // ------------------------------------------------------------------------
    class Component {
    public:
        virtual                        ~Component();

    };
    
    // ------------------------------------------------------------------------
    struct ComponentMeta {
        std::shared_ptr< Component > m_data;
        uint64                        m_mask;

    };

};

#endif
