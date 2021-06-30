cmake_minimum_required (VERSION 3.8)

project ("Crystal2")

add_executable (Crystal2 "Crystal2.cpp"
"src/Loaders/SceneLoader.h" 
"src/Loaders/SceneLoader.cpp" 
"src/Engine.cpp" 
"src/Engine.h"
"src/Core/RayTracer.h"
"src/Core/Scene.h"
"src/Core/Scene.cpp"
"src/Core/RayTracer.cpp"
"src/Core/FrameBuffer.h"
"src/Core/FrameBuffer.cpp"
"src/Cameras/Camera.h"
"src/Cameras/Camera.cpp"
"src/Cameras/PerspCamera.h"
"src/Cameras/PerspCamera.cpp"
"src/Accelerators/Accelerator.cpp"
"src/Shapes/Shape.h"

"src/Cameras/PerspCamera.h"
"src/Integrators/Integrator.h"
"src/Integrators/Integrator.cpp"
"src/Integrators/WhittedIntegrator.h"
"src/Integrators/SamplerIntegrator.h"
"src/Integrators/SamplerIntegrator.cpp"
"src/Samplers/Sampler.h"
"src/Samplers/DefaultSampler.h"
"src/Samplers/DefaultSampler.cpp"
"src/Core/Geometry.h"
"src/Core/Geometry.cpp"
"src/Integrators/WhittedIntegrator.cpp"
"src/Core/SurfaceInteraction.h"
"src/Shapes/Sphere.h"
"src/Shapes/Sphere.cpp"
"src/Core/Utils.h"
"src/Core/Utils.cpp"
"src/Accelerators/Brute.h"
"src/Accelerators/Brute.cpp"
"src/Core/SurfaceInteraction.cpp"

"src/Loaders/ConfigLoader.h"
"src/Loaders/ConfigLoader.cpp"
"src/Loaders/JsonLoader.h"
"src/Loaders/JsonLoader.cpp"
"src/Core/ConfigInfo.h"
"src/Crystal2.h"
"src/Samplers/Sampler.cpp"
"deps/stbi/stb_image.h"
"deps/stbi/stb_image_write.h"
"src/ToneMapper/ToneMapper.h"
"src/Samplers/StratifiedSampler.cpp"
"src/Shapes/Triangle.cpp"
"src/Shapes/Triangle.h"
"src/Shapes/TriangleMesh.h"
"src/Shapes/TriangleMesh.cpp"
"src/Loaders/ObjLoader.cpp"
"src/Loaders/ObjLoader.h"
"src/Accelerators/BVH.h"
"src/Accelerators/BVH.cpp"
"src/Core/Prototype.h"
"src/Core/Prototype.cpp"
"src/Materials/Material.h"
"src/Materials/DiffuseMaterial.h"
"src/Materials/DiffuseMaterial.cpp"
"src/BSDFs/BSDF.h"
"src/BSDFs/Lambertain.h"
"src/BSDFs/Lambertain.cpp"
"src/BSDFs/BSDF.cpp"
"src/Materials/Material.cpp"
"src/Materials/MirrorMaterial.h"
"src/Materials/MirrorMaterial.cpp"
"src/BSDFs/SpecularReflection.h"
"src/BSDFs/SpecularReflection.cpp" 
"src/BSDFs/SpecularTransmission.h" 
"src/BSDFs/SpecularTransmission.cpp" "src/Materials/Glass.h" "src/Materials/Glass.cpp" "src/Materials/MicrofacetMaterial.cpp" "src/Lights/Light.h" "src/Lights/Light.cpp")

set_target_properties (Crystal2 PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )

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