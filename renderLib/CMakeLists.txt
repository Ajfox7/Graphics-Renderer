add_library(renderLib
    BlinnPhongShader.h
    Camera.h
    Framebuffer.cpp
    Framebuffer.h
    HitRecord.h
    ISceneLoader.h
    LambertianShader.h
    PerspectiveCamera.h
    DiffuseShader.h
    EmitterShader.h
    MirrorShader.h
    Ray.h
    Scene.h
    SceneLoader.h
    SceneParser_JSON.cpp
    SceneParser_JSON.h
    Shader.h
    Shape.h
    Sphere.h
    Triangle.h
    vec3.h
)

target_include_directories(renderLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(renderLib
    PUBLIC
        PNG::PNG
        ZLIB::ZLIB
)
