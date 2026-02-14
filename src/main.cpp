#include "Scene.h"
#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include "Framebuffer.h"

int main() {
    PerspectiveCamera camera(
        vec3(0, 0, 5),          // origin
        vec3(0, 0, -1),         // view direction
        vec3(0, 1, 0),          // up
        1.0f,                   // focal length
        2.0f,                   // image plane width
        2.0f,                   // image plane height
        800, 600                // nx, ny
    );

    Scene scene(camera, PointLight(vec3(0, 10, 5), vec3(1, 1, 1)));

    auto redLambert = std::make_shared<Lambertian>(vec3(1, 0, 0));
    auto shinyBlue  = std::make_shared<BlinnPhong>(vec3(0, 0, 1), vec3(1, 1, 1), 32.0f);
    auto greenLambert = std::make_shared<BlinnPhong>(vec3(0, 1, 0),vec3(1,1,1), 50.0f);

    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(-1, 0, 0), redLambert));
    scene.addObject(std::make_shared<Sphere>( 1.0f, vec3( 1, 0, 0), shinyBlue));
    scene.addObject(std::make_shared<Triangle>(vec3(-4, -2, -1),  vec3( 4, -2, -1), vec3( 0,  4, -1), greenLambert));


    Framebuffer fb(800,600);

    for (int j = 0; j < fb.getHeight(); ++j) {
        for (int i = 0; i < fb.getWidth(); ++i) {

            Ray r = camera.generateRay(i, j);

            HitRecord rec;
            vec3 pixelColor(0.529, 0.808, 0.922);

            if (scene.hit(r, 0.001f, FLT_MAX, rec)) {
                pixelColor = rec.shader->rayColor(rec);
            }

            fb.setPixelColor(i, j, pixelColor);
        }
    }
    fb.exportAsPNG("output.png");
};