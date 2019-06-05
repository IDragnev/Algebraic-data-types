
namespace IDragnev::Meta
{
	template <typename Head, typename... Tail>
	Tuple<Head, Tail...>::Tuple(const Head& head, const Tuple<Tail...>& tail) :
		HeadElement(head),
		Tuple<Tail...>(tail)
	{
	}

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
}