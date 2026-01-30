#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "renderLib/Ray.h"

TEST_CASE( "Ray Origin and Direction" )
{
    point3 origin(1.0f, 2.0f, 3.0f);
    vec3 direction(4.0f, 5.0f, 6.0f);
    Ray ray(origin, direction);

    REQUIRE_THAT(ray.origin().x(), Catch::Matchers::WithinAbs(origin.x(), 1.0e-3f));
    REQUIRE_THAT(ray.origin().y(), Catch::Matchers::WithinAbs(origin.y(), 1.0e-3f));
    REQUIRE_THAT(ray.origin().z(), Catch::Matchers::WithinAbs(origin.z(), 1.0e-3f));
    REQUIRE_THAT(ray.direction().x(), Catch::Matchers::WithinAbs(direction.x(), 1.0e-3f));
    REQUIRE_THAT(ray.direction().y(), Catch::Matchers::WithinAbs(direction.y(), 1.0e-3f));
    REQUIRE_THAT(ray.direction().z(), Catch::Matchers::WithinAbs(direction.z(), 1.0e-3f));
}

TEST_CASE( "Ray at(t) Function" )
{
    point3 origin(0.0f, 0.0f, 0.0f);
    vec3 direction(1.0f, 2.0f, 3.0f);
    Ray ray(origin, direction);

    float t = 2.0f;
    float t2 = -7.3f;
    float t3 = 0.0f;
    point3 expectedPoint = origin + t * direction;
    point3 computedPoint = ray.at(t);
    point3 expectedPoint2 = origin + t2 * direction;
    point3 computedPoint2 = ray.at(t2);
    point3 expectedPoint3 = origin + t3 * direction;
    point3 computedPoint3 = ray.at(t3);

    REQUIRE_THAT(computedPoint.x(), Catch::Matchers::WithinAbs(expectedPoint.x(), 1.0e-3f));
    REQUIRE_THAT(computedPoint.y(), Catch::Matchers::WithinAbs(expectedPoint.y(), 1.0e-3f));
    REQUIRE_THAT(computedPoint.z(), Catch::Matchers::WithinAbs(expectedPoint.z(), 1.0e-3f));
    REQUIRE_THAT(computedPoint2.x(), Catch::Matchers::WithinAbs(expectedPoint2.x(), 1.0e-3f));
    REQUIRE_THAT(computedPoint2.y(), Catch::Matchers::WithinAbs(expectedPoint2.y(), 1.0e-3f));
    REQUIRE_THAT(computedPoint2.z(), Catch::Matchers::WithinAbs(expectedPoint2.z(), 1.0e-3f)); 
    REQUIRE_THAT(computedPoint3.x(), Catch::Matchers::WithinAbs(expectedPoint3.x(), 1.0e-3f));
    REQUIRE_THAT(computedPoint3.y(), Catch::Matchers::WithinAbs(expectedPoint3.y(), 1.0e-3f));
    REQUIRE_THAT(computedPoint3.z(), Catch::Matchers::WithinAbs(expectedPoint3.z(), 1.0e-3f)); 
} 

TEST_CASE( "Ray Immutability" )
{
    point3 origin(1.0f, 2.0f, 3.0f);
    vec3 direction(4.0f, 5.0f, 6.0f);
    Ray ray(origin, direction);

    // Attempt to modify origin and direction through the returned references
    const point3& origRef = ray.origin();
    const vec3& dirRef = ray.direction();

    // The following lines should cause compilation errors if uncommented
    // origRef.e[0] = 10.0f; 
    // dirRef.e[1] = 20.0f; 

    REQUIRE_THAT(ray.origin().x(), Catch::Matchers::WithinAbs(origin.x(), 1.0e-3f));
    REQUIRE_THAT(ray.direction().y(), Catch::Matchers::WithinAbs(direction.y(), 1.0e-3f));
    
}