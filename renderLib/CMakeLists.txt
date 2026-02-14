add_library(renderLib
    BlinnPhongShader.h
    Camera.h
    Framebuffer.cpp
    Framebuffer.h
    hit_record.h
    LambertianShader.h
    PerspectiveCamera.h
    Ray.h
    Scene.h
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
