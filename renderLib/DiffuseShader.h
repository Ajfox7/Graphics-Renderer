#pragma once
#include "Shader.h"

class Diffuse: public Shader {
    public:
        vec3 albedo;

        Diffuse(vec3 albedo) : albedo(albedo) {}

        vec3 rayColor (const HitRecord& h, const Scene& scene, int depth) const override {
            vec3 normal = h.normal;
            vec3 bounceDir;

            while(true){
                vec3 v = random_in_unit_sphere();
                float len2 = dot(v, v);
                if(len2 > 0.0f){
                    bounceDir = unit_vector(v);

                    if(dot(bounceDir, normal) < 0.0f) {
                        bounceDir = -bounceDir;
                    }
                    break;
                }
            }
            
            Ray scattered(h.point + 0.001f * normal, bounceDir);

            return albedo * scene.computeRayColor(scattered, depth);
        }
};