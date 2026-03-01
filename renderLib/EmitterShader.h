#pragma once
#include "Shader.h"

class Emitter : public Shader {
    public:
        vec3 color;

        Emitter(const vec3& color) {
            this->color = color;
        }

        virtual vec3 rayColor( const HitRecord& hit, const Scene& scene, int depth) const override {
            return color;
        }
};