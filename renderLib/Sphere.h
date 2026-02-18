#pragma once

#include "Shape.h"

class Sphere: public Shape {
    public:
        Sphere(): radius(1.0f), center(point3(0,0,-1)) {}

        Sphere(float r, const vec3& c)
        : center(c), radius(r), shader(nullptr) {}

        Sphere(float radius, point3 center, std::shared_ptr<Shader> s) : radius(radius), center(center), shader(s){}

        bool intersect(const Ray& r,const float tmin, float& tmax, HitRecord& rec) const override {
            vec3 oc = r.origin() - center;
            float a = dot(r.direction(), r.direction());
            float half_b = dot(oc, r.direction());
            float c = dot(oc, oc) - radius * radius;
            float discriminant = half_b * half_b - a * c;

            if (discriminant < 0) {
                return false;
            }

            float sqrt_discriminant = sqrt(discriminant);

            float root = (-half_b - sqrt_discriminant) / a;
            if (root < tmin || root > tmax) {
                root = (-half_b + sqrt_discriminant) / a;
                if (root < tmin || root > tmax)
                    return false;
            }

            rec.t = root;
            rec.point = r.at(root);

            vec3 outwardNormal = (rec.point - center) / radius;
            rec.setFaceNormal(r, outwardNormal);

            rec.shape = std::const_pointer_cast<Shape>(shared_from_this());
            rec.shader = shader;

            return true;
        }

        //getters
        float getRadius() const { return radius; }
        point3 getCenter() const { return center; }

    private:
        float radius;
        point3 center;
        std::shared_ptr<Shader> shader;
};