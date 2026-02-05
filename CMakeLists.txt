cmake_minimum_required(VERSION 3.20)
project(CPUScheduling LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ملاحظة: هذا المشروع يستخدم #include بين ملفات .cpp
# لذلك نحتاج فقط لترجمة main.cpp
# main.cpp يتضمن كل الملفات الأخرى عبر سلسلة includes

# Main executable - only main.cpp
add_executable(CPUScheduling main.cpp)

# Output directory
set_target_properties(CPUScheduling PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Copy data files
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
