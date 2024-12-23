if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	add_definitions(/DART_64BIT)
endif()

set(ART_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/ART/common/heap.cpp
	${CMAKE_SOURCE_DIR}/ART/common/memory.cpp
	${CMAKE_SOURCE_DIR}/ART/common/string.cpp

	${CMAKE_SOURCE_DIR}/ART/containers/vector.cpp
)