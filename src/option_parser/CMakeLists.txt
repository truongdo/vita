add_library(option_parser SHARED OptionParser.cpp)
target_include_directories (option_parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
