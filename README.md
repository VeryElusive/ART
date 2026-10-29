
Light weight low level runtime library written in modern C++. (work in progress)

Support for Windows and Linux. 

Define `ART_THREAD_SAFE_CPU_INIT` when compiling `common/memory.cpp` to synchronize
CPU-feature detection across `Memcpy`, `Memmove`, `Memset`, and `Memcmp`. This uses
the lightweight once primitive and needs no static TLS or OS synchronization calls.
Without the definition, the existing CPU-feature initialization path is retained.
