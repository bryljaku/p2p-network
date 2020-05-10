set(BOOST_ROOT "../../../boost-built-1_73_0")
set (Boost_USE_STATIC_LIBS OFF)
find_package (Boost REQUIRED COMPONENTS unit_test_framework)
include_directories (${Boost_INCLUDE_DIRS})

file(GLOB SOURCE_FILES "./*")
add_executable (tests-server ${SOURCE_FILES})
target_link_libraries (tests-server ${Boost_LIBRARIES})