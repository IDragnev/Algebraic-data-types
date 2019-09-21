
#include "Meta/ListAlgorithms.hpp"

namespace IDragnev::Detail
{
    struct DeduceResultType;

    template <typename R,
              typename Visitor,
              typename... Types
    > struct VisitResultT
    {
        using type = R;
    };

    template <typename Visitor, 
              typename... Types
    > struct VisitResultT<DeduceResultType, Visitor, Types...>
    {
        using type = std::common_type_t<std::invoke_result_t<Visitor, Types>...>;
    };

    template <typename R,
              typename Visitor,
              typename... Types
    > using VisitResult = typename VisitResultT<R, Visitor, Types...>::type;
}