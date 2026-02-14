#pragma once
#include <memory>
#include "Ray.h"
#include "hit_record.h"

class Shape: public std::enable_shared_from_this<Shape> {
    public:
        virtual ~Shape() = default;

        virtual bool intersect(const Ray& r,const float tmin, float& tmax, HitRecord& rec) const = 0;

        std::shared_ptr<Shader> shader;
};

