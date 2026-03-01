#include "Scene.h"
#include "PerspectiveCamera.h"
#include "Sphere.h"
#include "Triangle.h"
#include "LambertianShader.h"
#include "BlinnPhongShader.h"
#include "MirrorShader.h"
#include "DiffuseShader.h"
#include "Framebuffer.h"
#include "handleGraphicsArgs.h"
#include "EmitterShader.h"
#include <random>
#include <omp.h>
#include <chrono>

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    // Parse command-line arguments
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    float imagePlaneWidth = 2.0f;
    float imagePlaneHeight = imagePlaneWidth / args.aspectRatio;
    PerspectiveCamera camera(
        vec3(0, -2, 5),          // origin
        vec3(0, 0, -1),         // view direction
        vec3(0, 1, 0),          // up
        1.5f,                   // focal length
        imagePlaneWidth,                   // image plane width
        imagePlaneHeight,                   // image plane height
        args.width, args.height                // nx, ny
    );

    Scene scene(camera, PointLight(vec3(0, 10, 5), vec3(0.8, 0.8, 1)));

    //Create shaders
    auto redLambert = std::make_shared<Diffuse>(vec3(1, 0, 0));
    auto greenLambert = std::make_shared<Diffuse>(vec3(0, 1, 0));
    auto blueLambert = std::make_shared<Diffuse>(vec3(0, 0, 1)); 
    auto shinyBlue  = std::make_shared<BlinnPhong>(vec3(0, 0, 1), vec3(1, 1, 1), 32.0f);
    auto shinyGreen = std::make_shared<BlinnPhong>(vec3(0, 1, 0),vec3(1,1,1), 50.0f);
    auto bgLambert = std::make_shared<Lambertian>(vec3(0.2f, 0.2f, 0.2f));
    auto mirrorShader = std::make_shared<Mirror>();
    auto emitterShader = std::make_shared<Emitter>(vec3(100, 10, 100)); // Bright light source

    // Add objects to the scene
    scene.addObject(std::make_shared<Sphere>(1000.0f, vec3(0.0f, -1000.0f, -100.0f), bgLambert));
    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(-1.5, -1.5, -1), redLambert));
    scene.addObject(std::make_shared<Sphere>( 1.0f, vec3( 1.5, -1.5, -1), shinyBlue));
    scene.addObject(std::make_shared<Triangle>(vec3(-4, 2, -3),  vec3( 4, 2, -3), vec3( 0,  -4, -3), greenLambert));
    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(0, 0, -1), emitterShader));
    scene.addObject(std::make_shared<Sphere>(1.0f, vec3(0, -3, -1), mirrorShader));

    Framebuffer fb(args.width, args.height);

    // Stratified sampling parameters: `strata` per-dimension (strata*strata samples/pixel)
    const int strata = args.rpp;// change to 1 for no AA, 2 => 4 samples/pixel, 3 => 9, etc.
    const int samplesPerPixel = strata * strata;

    const int maxDepth = args.recursionDepth; // Max recursion depth for ray tracing
    
    #pragma omp parallel
    {
        #pragma omp single
        std::cout << "Using " << omp_get_num_threads() << " threads\n";

        std::mt19937 rng(std::random_device{}() ^ omp_get_thread_num());
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        #pragma omp for schedule(dynamic, 1)
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
    }

    fb.exportAsPNG(args.outputFileName);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Render time: " << elapsed.count() << " seconds\n";

    return 0;
};