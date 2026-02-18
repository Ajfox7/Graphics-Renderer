#pragma once
#include "Shader.h"

class Lambertian : public Shader {
    public:
        vec3 albedo;

        Lambertian(const vec3& color)
        : albedo(color) {}

        vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) override {
            
            vec3 L = unit_vector(scene.light.position - h.point);
            float NdotL = std::max(dot(h.normal, L), 0.0f);

            if(h.inShadow) {
                return vec3(0, 0, 0) + albedo * scene.light.color * NdotL * 0.5f; 
            }

            return albedo * scene.light.color * NdotL;
        }
};