#pragma once
#include "vec3.h"
#include "Ray.h"
#include "hit_record.h"

class Shader {
    public:
        virtual ~Shader() = default;

        virtual vec3 rayColor(const HitRecord& h) = 0;

};