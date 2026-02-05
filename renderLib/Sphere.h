#pragma once

#include "Shape.h"
#include "vec3.h"

class Sphere: public Shape {
    public:
        Sphere(): radius(1.0f), center(point3(0,0,-1)) {}

        Sphere(float radius, point3 center) : radius(radius), center(center) {}

        bool intersect(const Ray& r) override {
            vec3 oc = r.origin() - center;
            float a = dot(r.direction(), r.direction());
            float b = 2.0f * dot(oc, r.direction());
            float c = dot(oc, oc) - radius * radius;
            float discriminant = b * b - 4 * a * c;
            return (discriminant >= 0);
        }

        //getters
        float getRadius() const { return radius; }
        point3 getCenter() const { return center; }

    private:
        float radius;
        point3 center;
};