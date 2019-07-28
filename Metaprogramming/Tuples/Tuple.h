#pragma once

#include "TupleElement.h"
#include "Lists\ValueList.h"
#include "Lists\TypeList.h"

namespace IDragnev::Meta
{
	template <typename... Types>
	class Tuple;

	namespace Detail
	{
		template <typename T>
		struct TupleSize { };

		template <typename... Types>
		struct TupleSize<Tuple<Types...>> : CTValue<unsigned, sizeof...(Types)> { };

		template <typename T>
		inline constexpr unsigned tupleSize = TupleSize<T>::value;

		template <typename T, typename = std::void_t<>>
		struct IsTuple : std::false_type { };

		template <typename T>
		struct IsTuple<T, std::void_t<decltype(TupleSize<std::decay_t<T>>::value)>> : std::true_type { };

		template <typename T>
		inline constexpr bool isTuple = IsTuple<T>::value;
	}

	template <unsigned I,
		      typename TupleT, 
		      unsigned Size = Detail::tupleSize<std::decay_t<TupleT>>
	> inline constexpr
	decltype(auto) get(TupleT&& tuple) noexcept
	{
		return Detail::getValue<Size - I - 1>(std::forward<TupleT>(tuple));
	}

	template <>
	class Tuple<> { };

	template<typename Head, typename... Tail>
	class Tuple<Head, Tail...>
		: private Detail::TupleElement<sizeof...(Tail), Head>, 
		  private Tuple<Tail...>
	{
	private:
		using HeadElement = Detail::TupleElement<sizeof...(Tail), Head>;
		using TailTuple = Tuple<Tail...>;

		template <typename... Args>
		using EnableIfMatchesTailLength = std::enable_if_t<sizeof...(Args) == sizeof...(Tail)>;

	public:
		Tuple() = default;
		Tuple(Tuple&& source) = default;
		Tuple(const Tuple& source) = default;
		~Tuple() = default;

		template <typename VHead,
			      typename... VTail,
			      typename = EnableIfMatchesTailLength<VTail...>
		> constexpr Tuple(VHead&& head, VTail&&... tail);

		template <typename VHead,
			      typename... VTail,
			      typename = EnableIfMatchesTailLength<VTail...>
		> constexpr Tuple(const Tuple<VHead, VTail...>& source);

		template <typename VHead,
			      typename... VTail,
			      typename = EnableIfMatchesTailLength<VTail...>
		> constexpr Tuple(Tuple<VHead, VTail...>&& source);

		Tuple& operator=(Tuple&& rhs) = default;
		Tuple& operator=(const Tuple& rhs) = default;
		 
		template <typename VHead,
			      typename... VTail,
			      typename = EnableIfMatchesTailLength<VTail...>
		> constexpr Tuple& operator=(const Tuple<VHead, VTail...>& source);

		template <typename VHead,
			      typename... VTail,
			      typename = EnableIfMatchesTailLength<VTail...>
		> constexpr Tuple& operator=(Tuple<VHead, VTail...>&& source);

		constexpr Head& getHead() noexcept;
		constexpr const Head& getHead() const noexcept;

		constexpr TailTuple& getTail() noexcept;
		constexpr const TailTuple& getTail() const noexcept;
	
		template <unsigned I, typename TupleT, unsigned Size>
		friend constexpr decltype(auto) get(TupleT&& tuple) noexcept;
	};
}

#include "TupleImpl.hpp"
#include "TupleUtilities.h"