
include_directories("include/" ${Boost_INCLUDE_DIRS})
set(CMAKE_VERBOSE_MAKEFILE true )

set(SRC_FILE "src/stringutil.cpp" "src/raw_ostream.cpp")
set(TEST_FILE "test/stringutil_test.cpp" "test/raw_ostream_test.cpp")

add_library("kernel" ${SRC_FILE})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_definitions (-DBOOST_TEST_DYN_LINK)

foreach(iter ${TEST_FILE})
get_filename_component(TName ${iter} NAME_WE)
    add_executable("${TName}" ${iter})
    target_link_libraries("${TName}"
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    "kernel"
    )
    add_test("${TName}" "./${TName}")
endforeach(iter)
