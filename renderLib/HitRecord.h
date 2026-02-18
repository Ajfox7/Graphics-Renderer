#pragma once

#include <memory>

class Shape;
class Shader;

struct HitRecord {
    vec3 point;
    vec3 normal;
    float t = 0.0;
    bool frontFace = true;
    std::shared_ptr<Shape> shape;
    std::shared_ptr<Shader> shader;
    vec3 viewDir;
    bool inShadow = false;

    void setFaceNormal(const Ray& r, const vec3& outwardNormal) {
        frontFace = dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};