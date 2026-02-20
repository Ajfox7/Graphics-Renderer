#pragma once

#include "Shader.h"

class Mirror : public Shader {
    public:
        Mirror() {}

        vec3 rayColor (const HitRecord& h, const Scene& scene, int depth) const override {
            vec3 incoming = -h.viewDir;
            vec3 normal = h.normal;

            vec3 reflectedDir = incoming - 2 * dot(incoming, normal) * normal;
            reflectedDir = unit_vector(reflectedDir);

            // Create a new ray starting from the hit point in the reflected direction
            Ray reflectedRay(h.point + 0.001f * normal, reflectedDir); // Offset to avoid self-intersection
            
            return scene.computeRayColor(reflectedRay, depth);
        }
};