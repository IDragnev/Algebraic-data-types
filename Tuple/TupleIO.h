#pragma once

#include "TupleAlgorithms.h"
#include <iostream>

namespace IDragnev
{
    inline std::ostream& operator<<(std::ostream& out, const Tuple<>&)
    {
        out << "()";

        return out;
    }

    template <typename... Ts>
    std::ostream& operator<<(std::ostream& out, const Tuple<Ts...>& tuple)
    {
        out << '(' << tuple.getHead();
        forEach(tuple.getTail(), [&out](const auto& e) { out << ', ' << e; });
        out << ')';
        
        return out;
    }
}

