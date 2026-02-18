#pragma once

#include "Shape.h"

class Triangle: public Shape {
    public:
        Triangle(const point3& v0, const point3& v1, const point3& v2): v0(v0), v1(v1), v2(v2), shader(nullptr) {}
        Triangle(const point3& v0, const point3& v1, const point3& v2, std::shared_ptr<Shader> s)
            : v0(v0), v1(v1), v2(v2), shader(s) {}

        bool intersect(const Ray& r,const float tmin, float& tmax, HitRecord& rec) const override {
            vec3 edge1 = v1 - v0;
            vec3 edge2 = v2 - v0;

            vec3 h = cross(r.direction(), edge2);
            float a = dot(edge1, h);

            if (fabs(a) < 1e-8) return false;

            float f = 1.0f / a;
            vec3 s = r.origin() - v0;
            float u = f * dot(s, h);
            if (u < 0.0f || u > 1.0f) return false;

            vec3 q = cross(s, edge1);
            float v = f * dot(r.direction(), q);
            if (v < 0.0f || u + v > 1.0f) return false;

            float t = f * dot(edge2, q);
            if (t < tmin || t > tmax) return false;

            rec.t = t;
            rec.point = r.at(t);

            vec3 outwardNormal = unit_vector(cross(edge1, edge2));
            rec.setFaceNormal(r, outwardNormal);

            rec.shape = std::const_pointer_cast<Shape>(shared_from_this());
            rec.shader = shader;

            return true;
        }
    private:
        point3 v0;
        point3 v1;
        point3 v2;
        std::shared_ptr<Shader> shader;
};