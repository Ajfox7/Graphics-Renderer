#pragma once

#include "vec3.h"
#include "HitRecord.h"

class Scene;

class Shader {
    public:
        virtual ~Shader() = default;

        virtual vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) const = 0;

};