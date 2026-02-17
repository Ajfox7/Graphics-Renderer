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

        vec3 rayColor(const HitRecord& h) override {
            vec3 L = unit_vector(h.lightPos - h.point);
            vec3 H = unit_vector(L + h.viewDir);

            float NdotL = std::max(dot(h.normal, L), 0.0f);
            float NdotH = std::max(dot(h.normal, H), 0.0f);

            vec3 diffuse  = diffuseColor  * h.lightColor * NdotL;
            vec3 specular = specularColor * h.lightColor * std::pow(NdotH, shininess);

            if(h.inShadow) {
                return vec3(0, 0, 0) + diffuse * 0.5f; // simple shadow attenuation (reduce diffuse, remove specular)
            }

            return diffuse + specular;
        }
};