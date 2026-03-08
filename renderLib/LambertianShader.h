#pragma once
#include "Shader.h"

class Lambertian : public Shader {
    public:
        vec3 albedo;

        Lambertian(const vec3& color)
        : albedo(color) {}

        vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) const override {
            
            vec3 totalColor(0, 0, 0);
            
            for(const auto& light : scene.lights) {

                if(scene.isInShadow(h.point, light)) {
                    continue;
                }
            
                vec3 L = unit_vector(light->position - h.point);

                float NdotL = std::max(dot(h.normal, L), 0.0f);

                totalColor = totalColor + albedo * light->color * NdotL;
            }
            return totalColor;
        }
};