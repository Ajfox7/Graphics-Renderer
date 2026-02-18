#pragma once

class Scene;
struct HitRecord;

class Shader {
    public:
        virtual ~Shader() = default;

        virtual vec3 rayColor(const HitRecord& h, const Scene& scene, int depth) = 0;

};