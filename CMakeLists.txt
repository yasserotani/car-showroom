cmake_minimum_required(VERSION 3.10)
project(CarShowroom2025)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------
# 1) اجمع ملفات الهيدر
# ---------------------------------------------------------
file(GLOB_RECURSE HEADER_FILES include/**/*.hpp include/**/*.h)
source_group("Header Files" FILES ${HEADER_FILES})

# ---------------------------------------------------------
# 2) اجمع ملفات cpp
# ---------------------------------------------------------
file(GLOB_RECURSE SRC_FILES src/*.cpp src/**/*.cpp)

# أضف ملفات محددة يدويًا
list(APPEND SRC_FILES
    src/rendering/FloorRenderable.cpp
    src/rendering/WallRenderable.cpp
    src/halls/CenterHall.cpp
    src/glad.c        # ← GLAD أصبح داخل المشروع
)

# احذف الملفات القديمة
list(REMOVE_ITEM SRC_FILES src/rendering/GrassRenderable.cpp)
list(REMOVE_ITEM HEADER_FILES include/rendering/GrassRenderable.hpp)

# ---------------------------------------------------------
# 3) إعداد GLAD (الآن داخل المشروع)
# ---------------------------------------------------------
set(GLAD_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# ---------------------------------------------------------
# 4) إعداد GLFW (الآن داخل external/glfw)
# ---------------------------------------------------------
set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/external/glfw)

# ---------------------------------------------------------
# 5) أنشئ المشروع
# ---------------------------------------------------------
add_executable(CarShowroom2025 ${SRC_FILES} ${HEADER_FILES})

# ---------------------------------------------------------
# 6) مسارات التضمين
# ---------------------------------------------------------
target_include_directories(CarShowroom2025 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${GLAD_INCLUDE_DIR}
    ${GLFW_ROOT}/include
)

# ---------------------------------------------------------
# 7) ربط GLFW و OpenGL
# ---------------------------------------------------------
# إذا كنت تستخدم MSVC:
target_link_libraries(CarShowroom2025
    ${GLFW_ROOT}/lib/glfw3.lib
    opengl32
)

# إذا كنت تستخدم MinGW:
# target_link_libraries(CarShowroom2025
#     ${GLFW_ROOT}/lib/libglfw3.a
#     opengl32
#     gdi32
#     user32
#     kernel32
# )

# ---------------------------------------------------------
# 8) نسخ الشيدرز
# ---------------------------------------------------------
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
)

add_dependencies(CarShowroom2025 copy_shaders)

# ---------------------------------------------------------
# 9) نسخ التكستشرات
# ---------------------------------------------------------
add_custom_target(copy_textures ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/textures
        ${CMAKE_BINARY_DIR}/textures
)

add_dependencies(CarShowroom2025 copy_textures)

# Copy textures and shaders to output directory (Release/Debug) where executable runs
add_custom_command(TARGET CarShowroom2025 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets/textures
        $<TARGET_FILE_DIR:CarShowroom2025>/textures
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        $<TARGET_FILE_DIR:CarShowroom2025>/shaders
    COMMENT "Copying textures and shaders to output directory"
)