cmake_minimum_required (VERSION 3.8)

project ("Crystal2")

add_executable (Crystal2 "Crystal2.cpp"
"Crystal2.h" "src/Loader/SceneLoader.h" 
"src/Loader/SceneLoader.cpp" 
"src/Engine.cpp" 
"src/Loader/SceneInfo.h"
"src/Engine.h"
"src/Core/RayTracer.h"
"src/Core/Scene.h"
"src/Core/Scene.cpp"
"src/Core/RayTracer.cpp"
"src/Core/FrameBuffer.h"
"src/Core/FrameBuffer.cpp"
"src/Core/Camera.h" "src/Accelerator/Accelerator.cpp" "src/Shape/Shape.h" "src/Shape/Shape.cpp")

# 导入包含文件夹
include_directories (${PROJECT_SOURCE_DIR}/src)
include_directories (${PROJECT_SOURCE_DIR}/deps)

# 操作系统判定
if (UNIX)
    FIND_PACKAGE (glfw3)
    if (glfw3_FOUND)
        target_link_libraries(Crystal2 PRIVATE glfw)
    else()
        add_subdirectory("${PROJECT_SOURCE_DIR}/deps/GLFW")
        target_link_libraries(Crystal2 PRIVATE glfw)
    endif()
    target_link_libraries (Crystal2 PRIVATE dl X11 pthread)
else()
    add_subdirectory("${PROJECT_SOURCE_DIR}/deps/GLFW")
    target_link_libraries(Crystal2 PRIVATE glfw)
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(Crystal2 PRIVATE OpenGL::GL)


add_subdirectory ("deps/SJson")
add_subdirectory ("deps/glad")


target_link_libraries (Crystal2 PRIVATE SJson glad)