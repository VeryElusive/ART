if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_definitions(/DART_64BIT)
endif()

set(ART_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/ART/common/heap.cpp
	${PROJECT_SOURCE_DIR}/ART/common/memory.cpp
	${PROJECT_SOURCE_DIR}/ART/common/string.cpp

	${PROJECT_SOURCE_DIR}/ART/concurrency/spinlock.cpp

	${PROJECT_SOURCE_DIR}/ART/hash/luhash.cpp

	${PROJECT_SOURCE_DIR}/ART/input/input.cpp

	${PROJECT_SOURCE_DIR}/ART/sorting/sort.cpp

	${PROJECT_SOURCE_DIR}/ART/platform/file.cpp
)