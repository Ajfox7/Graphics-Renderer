#include "Scene.h"
#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include "MirrorShader.h"
#include "DiffuseShader.h"
#include "Framebuffer.h"
#include <random>

int main() {
    PerspectiveCamera camera(
        vec3(0, 0, 5),          // origin
        vec3(0, 0, -1),         // view direction
        vec3(0, 1, 0),          // up
        0.75f,                   // focal length
        2.0f,                   // image plane width
        2.0f,                   // image plane height
        600, 600                // nx, ny
    );

    Scene scene(camera, PointLight(vec3(0, 10, 5), vec3(0.8, 0.8, 1)));

    //Create shaders
    auto redLambert = std::make_shared<Diffuse>(vec3(1, 0, 0));
    auto shinyBlue  = std::make_shared<BlinnPhong>(vec3(0, 0, 1), vec3(1, 1, 1), 32.0f);
    auto shinyGreen = std::make_shared<BlinnPhong>(vec3(0, 1, 0),vec3(1,1,1), 50.0f);
    auto bgLambert = std::make_shared<Lambertian>(vec3(0.45f, 0.45f, 0.45f));
    auto mirrorShader = std::make_shared<Mirror>();
    
    // Add objects to the scene
    scene.addObject(std::make_shared<Sphere>(1000.0f, vec3(0.0f, -1000.0f, -100.0f), bgLambert));
    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(-4, 4, -1), redLambert));
    scene.addObject(std::make_shared<Sphere>( 1.0f, vec3( 4, 4, -1), shinyBlue));
    scene.addObject(std::make_shared<Triangle>(vec3(-4, 0, -3),  vec3( 4, 0, -3), vec3( 0,  6, -3), shinyGreen));
    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(0, 2, -1), mirrorShader));

    Framebuffer fb(600, 600);

    // Stratified sampling parameters: `strata` per-dimension (strata*strata samples/pixel)
    const int strata = 4;// change to 1 for no AA, 2 => 4 samples/pixel, 3 => 9, etc.
    const int samplesPerPixel = strata * strata;

    std::mt19937 rng(static_cast<unsigned int>(std::random_device{}()));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    const int maxDepth = 3; // Max recursion depth for ray tracing

    for (int j = 0; j < fb.getHeight(); ++j) {
        for (int i = 0; i < fb.getWidth(); ++i) {

            vec3 accumColor(0.0f, 0.0f, 0.0f);

            for (int sy = 0; sy < strata; ++sy) {
                for (int sx = 0; sx < strata; ++sx) {
                    float rx = dist(rng);
                    float ry = dist(rng);

                    float sampleX = i + (sx + rx) / static_cast<float>(strata);
                    float sampleY = j + (sy + ry) / static_cast<float>(strata);

                    Ray r = camera.generateRay(sampleX, sampleY);

                    vec3 sampleColor = scene.computeRayColor(r, maxDepth);

                    accumColor = accumColor + sampleColor;
                }
            }

            vec3 pixelColor = accumColor / static_cast<float>(samplesPerPixel);
            fb.setPixelColor(i, j, pixelColor);
        }
    }
    fb.exportAsPNG("output.png");
};