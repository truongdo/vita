set(CMAKE_BUILD_TYPE Debug)
find_package( Boost REQUIRED COMPONENTS program_options regex )
include_directories( ${Boost_INCLUDE_DIRS} )

add_library (text_util SHARED text_util.cpp)
add_executable(text_util_test text_util_test.cpp)

target_link_libraries( text_util ${Boost_LIBRARIES}  )
target_link_libraries( text_util_test ${Boost_LIBRARIES} text_util )
target_include_directories (text_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
