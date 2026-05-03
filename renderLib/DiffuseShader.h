#pragma once
#include "Shader.h"

class Diffuse: public Shader {
    public:
        vec3 albedo;

        Diffuse(vec3 albedo) : albedo(albedo) {}

        vec3 rayColor (const HitRecord& h, const Scene& scene, int depth) const override {
           
            vec3 bounceDir = h.normal + random_unit_vector();
            
            if (bounceDir.near_zero()) {
                bounceDir = h.normal;
            }

            Ray scattered(h.point + 0.001f * h.normal, unit_vector(bounceDir));
            return albedo * scene.computeRayColor(scattered, depth);
        }
};