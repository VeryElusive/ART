namespace ART {
    namespace CompileTime {
        template<typename T>
        concept SequenceTraits = requires { T::template next<>::value; };

        // instantiatable compile-time sequence, unique to each translation unit
        template<typename Id, SequenceTraits Base>
        struct Sequence {
        private:
            template<typename T>
            struct getter {
                friend auto flag(getter<T>);
            };

            template<typename T>
            struct setter {
                friend auto flag(getter<T>) { }
                using current = T;
            };

            // private helper
            // sequentially checks all possible types from the base, until one that has not been used is found
            template<auto Tag = []{}, SequenceTraits T = Base> 
            static consteval auto getCurrent() {
                if constexpr(requires(getter<T> r) { flag(r); })
                    return getCurrent<Tag, typename T::template next<>>();
                else return T();
            }

        public:
            template<auto Tag = []{}, SequenceTraits T = Base>
            using current = decltype(getCurrent<Tag, T>());

            template<auto Tag = []{}>
            using advance = setter<current<Tag>>::current;
        };

        template<auto current = 0ull>
        struct CounterSequenceTraits {
            template<typename = void>
            using next = CounterSequenceTraits<current + 1>;
            constexpr static auto value = current;
        };

        template<typename Id, auto base = 0ull>
        using Counter = Sequence<Id, CounterSequenceTraits<base>>;

        // global counter for any use -- works basically the same as the __COUNTER__ macro
        struct CounterGlobal : Counter<CounterGlobal> { };
    }
}
