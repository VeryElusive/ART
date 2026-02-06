namespace ART {
    namespace Template {
        template<typename T>
        struct GuardVar {
            const T backup;
            T& reference;

            bool persist = false;

            GuardVar() = delete;
            GuardVar(T& ref) : reference(ref), backup(ref) { }

            ~GuardVar() {
                if(!persist) reference = backup;
            }
        };
    }
}
