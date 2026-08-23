if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	if(MSVC)
		add_definitions(/DART_64BIT)
	else()
		add_definitions(-DART_64BIT)
	endif()
endif()

set(ART_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/ART/common/heap.cpp
	${PROJECT_SOURCE_DIR}/ART/common/memory.cpp
	${PROJECT_SOURCE_DIR}/ART/common/string.cpp

	${PROJECT_SOURCE_DIR}/ART/concurrency/spinlock.cpp
	${PROJECT_SOURCE_DIR}/ART/concurrency/threading.cpp

	${PROJECT_SOURCE_DIR}/ART/hash/luhash.cpp

	${PROJECT_SOURCE_DIR}/ART/input/input.cpp

	${PROJECT_SOURCE_DIR}/ART/platform/file.cpp
)

if(WIN32)
	list(APPEND ART_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/ART/concurrency/threading_windows.cpp
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	list(APPEND ART_SOURCE_FILES
		${PROJECT_SOURCE_DIR}/ART/concurrency/threading_linux.cpp
	)
	find_package(Threads REQUIRED)
	list(APPEND ART_LINK_LIBRARIES Threads::Threads)
endif()
