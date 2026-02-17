#pragma once
#include "Shader.h"
#include "hit_record.h"

class Lambertian : public Shader {
    public:
        vec3 albedo;

        Lambertian(const vec3& color)
        : albedo(color) {}

        vec3 rayColor(const HitRecord& h) override {
            vec3 L = unit_vector(h.lightPos - h.point);
            float NdotL = std::max(dot(h.normal, L), 0.0f);

            if(h.inShadow) {
                return vec3(0, 0, 0) + albedo * h.lightColor * NdotL * 0.5f; 
            }

            return albedo * h.lightColor * NdotL;
        }
};