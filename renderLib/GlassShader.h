#pragma once
#include "shader.h"

class Glass : public Shader {
    public:
        float refractionIndex;
        vec3 absorptionColor;

        Glass(float refractionIndex, vec3 absorb) : refractionIndex(refractionIndex), absorptionColor(absorb) {}

        vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) const override {
            vec3 unitIncoming = unit_vector(h.viewDir);
            vec3 normal = h.normal;

            float refractionRatio = h.frontFace ? (1.0f / refractionIndex) : refractionIndex;

            float cosTheta = fmin(dot(-unitIncoming, normal), 1.0f);
            float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

            bool cannotRefract = refractionRatio * sinTheta > 1.0f;
            vec3 direction;

            if(cannotRefract || reflectance(cosTheta, refractionRatio) > random_float()) {
                direction = reflect(unitIncoming, normal);
            }else{
                direction = refract(unitIncoming, normal, refractionRatio);
            }

            if (direction.near_zero()) {
                direction = h.normal;
            }

            vec3 origin = h.point + (dot(direction, h.normal) > 0 ? 0.001f : -0.001f) * h.normal;

            Ray scatteredRay(origin, direction);
            vec3 color = scene.computeRayColor(scatteredRay, depth);

            if (!h.frontFace) {
                // We just traveled through the glass to hit the back face
                // 'h.t' is the length of the ray segment inside the object
                float d = h.t; 
                vec3 attenuation = vec3(
                    exp(-absorptionColor.x() * d),
                    exp(-absorptionColor.y() * d),
                    exp(-absorptionColor.z() * d)
                );
                return attenuation * color;
            }

            return color;
        }
    private:
        static float reflectance(float cosine, float refIdx) {
            // Schlick's approximation for reflectance
            auto r0 = (1 - refIdx) / (1 + refIdx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }

        static vec3 reflect(const vec3& v, const vec3& n) {
            return v - 2 * dot(v, n) * n;
        }

        static vec3 refract(const vec3& uv, const vec3& n, float etai_over_etat) {
            float cos_theta = fmin(dot(-uv, n), 1.0f);
            
            // Perpendicular component
            vec3 r_out_perp =  etai_over_etat * (uv + cos_theta * n);
            
            // Parallel component
            float k = 1.0f - r_out_perp.length_squared();
            if (k < 0) return reflect(uv, n); // total internal reflection fallback
            vec3 r_out_parallel = -sqrt(k) * n;
            
            return r_out_perp + r_out_parallel;
        }
};