#pragma once

#include "Ray.h"

class Shape {
    public:
        Shape() {}
        virtual ~Shape() {}
        virtual bool intersect(const Ray& r) = 0;
};

