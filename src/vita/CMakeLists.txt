INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../build/crfsuite/installed/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../build//crfsuite/installed/lib ${CMAKE_CURRENT_SOURCE_DIR}/../../build/liblbfgs-1.10/installed/lib)
add_executable (vita vita.cpp)
target_link_libraries( vita LINK_PUBLIC text_util text2feat dict option_parser crfsuite vncommon)
