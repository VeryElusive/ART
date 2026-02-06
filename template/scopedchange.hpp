namespace ART {
    namespace Template {
        template<typename T>
        struct ScopedChange {
            const T backup;
            T& reference;

            ScopedChange() = delete;
            ScopedChange(T& ref, const T& value) : reference(ref), backup(ref) {
                ref = value;
            }

            ~ScopedChange() {
                reference = backup;
            }
        };
    }
}
