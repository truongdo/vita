INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../build/crfsuite/installed/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../build//crfsuite/installed/lib ${CMAKE_CURRENT_SOURCE_DIR}/../../build/liblbfgs-1.10/installed/lib)

add_executable(vita_ana vita_ana.cpp)
target_link_libraries( vita_ana LINK_PUBLIC text_util text2feat option_parser crfsuite cqdb lbfgs dict vncommon)
