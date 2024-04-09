cmake_minimum_required(VERSION 3.16)

project("SimpleValidator")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR})

add_library(simple_validator)

target_sources(simple_validator PRIVATE
"tag.cpp"
"tag_validator/open_close_tag_validator.cpp"
"text_validator/special_character_text_validator.cpp" 
"tag_validator/character_tag_validator.cpp" 
"tag_validator/xml_declaration_tag_validator.cpp")

add_executable(validator main.cpp)
target_link_libraries(validator simple_validator)

