#pragma once

#include "TupleElement.h"
#include "Lists\ValueList.h"

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
	}

	template <unsigned I, typename TupleT, 
		      unsigned Size = Detail::tupleSize<std::remove_const_t<TupleT>>>
	inline decltype(auto) get(TupleT& tuple) noexcept
	{
		return Detail::extractValue<Size - I - 1>(tuple);
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

		template <typename... VTail>
		using EnableIfMatchesTailLength = std::enable_if_t<sizeof...(VTail) == sizeof...(Tail)>;

	public:
		Tuple() = default;
		Tuple(Tuple&& source) = default;
		Tuple(const Tuple& source) = default;
		~Tuple() = default;

		template<typename VHead,
			     typename... VTail,
			     typename = EnableIfMatchesTailLength<VTail...>>
		Tuple(VHead&& head, VTail&&... tail);

		template<typename VHead,
			     typename... VTail,
			     typename = EnableIfMatchesTailLength<VTail...>>
		Tuple(const Tuple<VHead, VTail...>& source);

		template<typename VHead,
			     typename... VTail,
			     typename = EnableIfMatchesTailLength<VTail...>>
		Tuple(Tuple<VHead, VTail...>&& source);

		Tuple& operator=(Tuple&& rhs) = default;
		Tuple& operator=(const Tuple& rhs) = default;
		 
		template<typename VHead,
			     typename... VTail,
			     typename = EnableIfMatchesTailLength<VTail...>>
		Tuple& operator=(const Tuple<VHead, VTail...>& source);

		template<typename VHead,
			     typename... VTail,
			     typename = EnableIfMatchesTailLength<VTail...>>
		Tuple& operator=(Tuple<VHead, VTail...>&& source);

		Head& getHead() noexcept;
		const Head& getHead() const noexcept;

		Tuple<Tail...>& getTail() noexcept;
		const Tuple<Tail...>& getTail() const noexcept;
	
		template <unsigned I, typename TupleT, unsigned Size>
		friend decltype(auto) get(TupleT& tuple) noexcept;
	};

	template <typename... Types>
	auto makeTuple(Types&&... args);

	bool operator==(const Tuple<>&, const Tuple<>&) noexcept;

	template <typename H1, typename... Tail1,
		      typename H2, typename... Tail2,
		      typename = std::enable_if_t<sizeof...(Tail1) == sizeof...(Tail2)>>
	bool operator==(const Tuple<H1, Tail1...>& lhs, const Tuple<H2, Tail2...>& rhs);
}

#include "TupleImpl.hpp"