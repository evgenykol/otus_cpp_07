cmake_minimum_required(VERSION 3.2)

if(NOT DEFINED ENV{TRAVIS_BUILD_NUMBER})
    message("TRAVIS_BUILD_NUMBER environment set to default")
    set(ENV{TRAVIS_BUILD_NUMBER} "1")
endif()

project(bulk VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

configure_file(version.h.in version.h)

add_library(bulk_lib bulk.h bulk.cpp)

add_executable(bulk main.cpp)

#add_executable(bulk_tests bulk_tests.cpp)

set_target_properties(bulk bulk_lib
        PROPERTIES
                CXX_STANDARD 14
                CXX_STANDARD_REQUIRED ON
                CMAKE_CXX_EXTENSIONS OFF
                COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

#set_target_properties(bulk_tests
#        PROPERTIES
#                COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
#                INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
#)

target_link_libraries(bulk bulk_lib)

#target_link_libraries(bulk_tests
#        ${Boost_LIBRARIES}
#        bulk_lib
#)

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT edkolotilov@gmail.com)

include (CPack)

#enable_testing()

#add_test(check_print_ips_int ip_filter_tests)
