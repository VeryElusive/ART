if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_definitions(/DART_64BIT)
endif()

if(NOT ART_DEFINED)
	set(ART_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/ART/common/heap.cpp
		${PROJECT_SOURCE_DIR}/ART/common/memory.cpp
		${PROJECT_SOURCE_DIR}/ART/common/string.cpp

		${PROJECT_SOURCE_DIR}/ART/concurrency/spinlock.cpp

		${PROJECT_SOURCE_DIR}/ART/hash/luhash.cpp
	)

	set(ART_DEFINED ON)
else()
	set(ART_ALREADY_DEFINED ON)

	add_definitions(/DART_ALREADY_DEFINED)
endif()

