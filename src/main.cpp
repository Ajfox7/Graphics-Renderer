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
#include "SceneLoader.h"
#include "SceneParser_JSON.h"
#include <random>
#include <omp.h>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    // Parse command-line arguments
    sivelab::GraphicsArgs args;
    args.process(argc, argv);

    Scene scene;

    std::shared_ptr<ISceneLoader> loader = std::make_shared<SceneLoader>(scene);
    SceneParser_JSON parser(loader);
    parser.parseFileData(args.inputFileName);

    Framebuffer fb(args.width, args.height);
    scene.cameras[0]->setResolution(args.width, args.height);

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

                        Ray r = scene.cameras[0]->generateRay(sampleX, sampleY);
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