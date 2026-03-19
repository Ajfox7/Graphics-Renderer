#pragma once

#include "vec3.h"
#include "Ray.h"

class BoundingBox {
    public:

        BoundingBox(const vec3& min, const vec3& max) : llb(min), urf(max) {}

        BoundingBox merge(const BoundingBox& other) const {
            vec3 newLLB(std::min(llb.x(), other.llb.x()), std::min(llb.y(), other.llb.y()), std::min(llb.z(), other.llb.z()));
            vec3 newURF(std::max(urf.x(), other.urf.x()), std::max(urf.y(), other.urf.y()), std::max(urf.z(), other.urf.z()));
            return BoundingBox(newLLB, newURF);
        }

        bool hit(const Ray& r, float tmin, float tmax) const {
            for (int a = 0; a < 3; a++) {
                float invD = 1.0f / r.direction()[a];
                float t0 = (llb[a] - r.origin()[a]) * invD;
                float t1 = (urf[a] - r.origin()[a]) * invD;
                if (invD < 0.0f) std::swap(t0, t1);
                tmin = t0 > tmin ? t0 : tmin;
                tmax = t1 < tmax ? t1 : tmax;
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }

        // lower left back
        vec3 llb;
        // upper right front
        vec3 urf;
};