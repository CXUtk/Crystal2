cmake_minimum_required (VERSION 3.13)

project ("Crystal2")

add_executable (Crystal2 "Crystal2.cpp"
 
 
"src/CrystalEngine.cpp" 
"src/CrystalEngine.h"
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
"src/Integrators/PathTracingIntegrator.h"
"src/Integrators/SamplerIntegrator.h"
"src/Integrators/SamplerIntegrator.cpp"
"src/Samplers/Sampler.h"
"src/Samplers/DefaultSampler.h"
"src/Samplers/DefaultSampler.cpp"
"src/Core/Geometry.h"
"src/Core/Geometry.cpp"
"src/Integrators/PathTracingIntegrator.cpp"
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
"src/BSDFs/SpecularTransmission.cpp"
"src/Materials/Glass.h"
"src/Materials/Glass.cpp"
"src/Materials/MicrofacetMaterial.cpp"
"src/Lights/Light.h"
"src/Lights/Light.cpp"
"src/Lights/PointLight.h"
"src/Lights/PointLight.cpp"
"src/Lights/AreaLight.h"
"src/Lights/AreaLight.cpp"
"src/BSDFs/Models/Fresnel.h"
"src/BSDFs/Models/Fresnel.cpp"
"src/BSDFs/Models/MicrofacetDistribution.h"
"src/BSDFs/Models/MicrofacetDistribution.cpp"
"src/BSDFs/SpecularFresnel.h"
"src/BSDFs/SpecularFresnel.cpp" 
"src/BSDFs/Microfacet.h"
"src/BSDFs/Microfacet.cpp" 
"src/Textures/Texture.h"
"src/Textures/UVTexture.h"
"src/Textures/Texel.h"
"src/Textures/Texel.cpp"
"src/Textures/CubemapTexture.h" 
"src/Textures/CubemapTexture.cpp"
 "src/BSDFs/BlinnPhongReflection.h" "src/BSDFs/BlinnPhongReflection.cpp" "src/Materials/PhongMaterial.h" "src/Materials/PhongMaterial.cpp" "src/Lights/DiffusedAreaLight.h" "src/Lights/DiffusedAreaLight.cpp" "src/Core/Entities.h" "src/Core/Entities.cpp" "src/Integrators/DirectLightingIntegrator.h" "src/BSDFs/Reflections.h" "src/BSDFs/Reflections.cpp" "src/Demos/BRDFDemo.h" "src/Demos/BRDFDemo.cpp" "src/BSDFs/FresnelBlend.h" "src/BSDFs/FresnelBlend.cpp")

# 编译选项
set_target_properties("Crystal2" PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
            )


# 导入包含文件夹
include_directories(SYSTEM "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/deps" "${PROJECT_SOURCE_DIR}/deps/TREngine" "${PROJECT_SOURCE_DIR}/deps/TREngine/deps")

find_package(OpenGL REQUIRED)
target_link_libraries("Crystal2" PRIVATE OpenGL::GL)

add_subdirectory ("deps/TREngine")

target_link_libraries ("Crystal2" PRIVATE TRV2_Engine)

add_custom_command(TARGET "Crystal2" PRE_BUILD
                   COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")
execute_process(COMMAND python ${PROJECT_SOURCE_DIR}/build.py "${PROJECT_SOURCE_DIR}/Resources" "${PROJECT_BINARY_DIR}")