namespace IDragnev::Meta::Folds
{
    template <template <typename> typename Predicate,
              typename... Types
    > inline constexpr auto allOf = (Predicate<Types>::value && ...);

    template <template <typename> typename Predicate,
              typename... Types
    > inline constexpr auto anyOf = (Predicate<Types>::value || ...);
}