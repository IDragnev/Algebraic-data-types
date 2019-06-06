#pragma once

#include <type_traits>

namespace IDragnev::Meta
{
	namespace Detail
	{
		template <typename T>
		inline constexpr auto canBeInherited = std::is_class_v<T> && !std::is_final_v<T>;

		template <unsigned Height, 
			      typename T,
			      bool = canBeInherited<T>
		> class TupleElement;

		template <unsigned Height, typename T>
		class TupleElement<Height, T, false>
		{
		public:
			TupleElement() = default;
			template<typename U>
			TupleElement(U&& value) : value(std::forward<U>(value)) { }

			T& get() noexcept { return value; }
			const T& get() const noexcept { return value; }
			
		private:
			T value;
		};

		template<unsigned Height, typename T>
		class TupleElement<Height, T, true> : private T
		{
		public:
			TupleElement() = default;
			template<typename U>
			TupleElement(U&& value) : T(std::forward<U>(value)) { }

			T& get() noexcept { return *this; }
			const T& get() const noexcept { return *this; }
		};
	}

	template <typename... Types>
	class Tuple;

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

		Head& getHead() noexcept;
		const Head& getHead() const noexcept;

		Tuple<Tail...>& getTail() noexcept;
		const Tuple<Tail...>& getTail() const noexcept;
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