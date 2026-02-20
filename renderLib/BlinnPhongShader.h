#pragma once
#include "Shader.h"

class BlinnPhong : public Shader {
    public: 
        vec3 diffuseColor;
        vec3 specularColor;
        float shininess;

        BlinnPhong(const vec3& diffuse, const vec3& specular, float shininess)
        : diffuseColor(diffuse),
          specularColor(specular),
          shininess(shininess) {}

        vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) const override {
            
            vec3 L = unit_vector(scene.light.position - h.point);
            vec3 H = unit_vector(L + h.viewDir);

            float NdotL = std::max(dot(h.normal, L), 0.0f);
            float NdotH = std::max(dot(h.normal, H), 0.0f);

            vec3 diffuse  = diffuseColor  * scene.light.color * NdotL;
            vec3 specular = specularColor * scene.light.color * std::pow(NdotH, shininess);

            if(h.inShadow) {
                return vec3(0, 0, 0) + diffuse * 0.5f; 
            }

            return diffuse + specular;
        }
};