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

            vec3 totalDiffuse(0, 0, 0);
            vec3 totalSpecular(0, 0, 0);
            
            for(const auto& light : scene.lights) {

                if(scene.isInShadow(h.point, light)) {
                    continue; 
                }

                vec3 L = unit_vector(light->position - h.point);
                vec3 H = unit_vector(L + h.viewDir);

                float NdotL = std::max(dot(h.normal, L), 0.0f);
                float NdotH = std::max(dot(h.normal, H), 0.0f);

                totalDiffuse  = totalDiffuse + (diffuseColor  * light->color * NdotL);
                totalSpecular = totalSpecular + (specularColor * light->color * std::pow(NdotH, shininess));
            }
            return totalDiffuse + totalSpecular;
        }
};