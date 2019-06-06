
namespace IDragnev::Meta
{
	template <typename Head, typename... Tail>
	template <typename VHead, typename... VTail, typename>
	Tuple<Head, Tail...>::Tuple(VHead&& head, VTail&&... tail) :
		HeadElement(std::forward<VHead>(head)),
		TailTuple(std::forward<VTail>(tail)...)
	{
	}

	template <typename Head, typename... Tail>
	template <typename VHead, typename... VTail, typename>
	Tuple<Head, Tail...>::Tuple(const Tuple<VHead, VTail...>& source) :
		HeadElement(source.getHead()),
		TailTuple(source.getTail())
	{
	}

	template <typename Head, typename... Tail>
	template <typename VHead, typename... VTail, typename>
	Tuple<Head, Tail...>::Tuple(Tuple<VHead, VTail...>&& source) :
		HeadElement(std::move(source.getHead())),
		TailTuple(std::move(source.getTail()))
	{
	}

	template <typename Head, typename... Tail>
	template<typename VHead, typename... VTail, typename>
	auto Tuple<Head, Tail...>::operator=(const Tuple<VHead, VTail...>& rhs) -> Tuple&
	{
		getHead() = rhs.getHead();
		getTail() = rhs.getTail();

		return *this;
	}

	template <typename Head, typename... Tail>
	template<typename VHead, typename... VTail, typename>
	auto Tuple<Head, Tail...>::operator=(Tuple<VHead, VTail...>&& rhs) -> Tuple&
	{
		getHead() = std::move(rhs.getHead());
		getTail() = std::move(rhs.getTail());

		return *this;
	}

	template <typename Head, typename... Tail>
	inline Head& Tuple<Head, Tail...>::getHead() noexcept
	{
		return static_cast<HeadElement&>(*this).get();
	}

	template <typename Head, typename... Tail>
	inline const Head& Tuple<Head, Tail...>::getHead() const noexcept
	{
		return static_cast<const HeadElement&>(*this).get();
	}

	template <typename Head, typename... Tail>
	inline Tuple<Tail...>& Tuple<Head, Tail...>::getTail() noexcept 
	{
		return *this;
	}

	template <typename Head, typename... Tail>
	inline const Tuple<Tail...>& Tuple<Head, Tail...>::getTail() const noexcept
	{
		return *this;
	}

	template <typename... Types>
	inline auto makeTuple(Types&&... args)
	{
		using T = Tuple<std::decay_t<Types>...>;
		return T(std::forward<Types>(args)...);
	}

	template <typename H1, typename... Tail1,
		      typename H2, typename... Tail2,
		      typename>
	bool operator==(const Tuple<H1, Tail1...>& lhs, const Tuple<H2, Tail2...>& rhs)
	{
		return lhs.getHead() == rhs.getHead() &&
			   lhs.getTail() == rhs.getTail();
	}
	
	inline bool operator==(const Tuple<>&, const Tuple<>&) noexcept
	{
		return true;
	}
}