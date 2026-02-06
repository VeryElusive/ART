#include "../common/decl.hpp"

namespace ART {
    namespace CompileTime {
        // compile-time visitable type list
        template<typename... Ts>
        struct TypeList {
            // true means stop visiting
            template <typename F>
            static FORCEINLINE bool visit(F&& visitor) {
                return (visitor.template operator()<Ts>() || ...);
            }
        };
    }
}