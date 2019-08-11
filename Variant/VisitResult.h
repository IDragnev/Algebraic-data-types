
#include "Meta\List algorithms.h"

namespace IDragnev::Detail
{
    struct ComputeResultType;

    template <typename R,
              typename Visitor,
              typename... Types
    > struct VisitResultT
    {
        using type = R;
    };

    template <typename Visitor, 
              typename... Types
    > struct VisitResultT<ComputeResultType, Visitor, Types...>
    {
        using type = std::common_type_t<std::invoke_result_t<Visitor, Types>...>;
    };

    template <typename R,
              typename Visitor,
              typename... Types
    > using VisitResult = typename VisitResultT<R, Visitor, Types...>::type;
}