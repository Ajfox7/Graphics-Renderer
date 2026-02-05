#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <renderLib/Sphere.h>
#include <renderLib/Ray.h>
#include <renderLib/vec3.h>

TEST_CASE( "Sphere Intersection Test - Hit" )
{
    Sphere sphere(1.0f, point3(0,0,-5));
    Ray ray(point3(0,0,0), vec3(0,0,-1));

    bool hit = sphere.intersect(ray);

    REQUIRE(hit == true);
}
TEST_CASE( "Sphere Intersection Test - Miss" )
{
    Sphere sphere(1.0f, point3(0,0,-5));
    Ray ray(point3(0,0,0), vec3(0,1,0));

    bool hit = sphere.intersect(ray);

    REQUIRE(hit == false);
}
TEST_CASE( "Sphere Intersection Test - Tangent" )
{
    Sphere sphere(1.0f, point3(0,0,-5));
    Ray ray(point3(1,0,0), vec3(0,0,-1));

    bool hit = sphere.intersect(ray);

    REQUIRE(hit == true);
}
TEST_CASE( "Constructor Test" ){
    Sphere s = Sphere();
    Sphere s2 = Sphere(2.0f, point3(1,1,-3));

    REQUIRE_THAT(s.getCenter().x(), Catch::Matchers::WithinAbs(0.0f, 1e-3));
    REQUIRE_THAT(s.getCenter().y(), Catch::Matchers::WithinAbs(0.0f, 1e-3));
    REQUIRE_THAT(s.getCenter().z(), Catch::Matchers::WithinAbs(-1.0f, 1e-3));
    REQUIRE_THAT(s2.getCenter().x(), Catch::Matchers::WithinAbs(1.0f, 1e-3));
    REQUIRE_THAT(s2.getCenter().y(), Catch::Matchers::WithinAbs(1.0f, 1e-3));
    REQUIRE_THAT(s2.getCenter().z(), Catch::Matchers::WithinAbs(-3.0f, 1e-3));
    REQUIRE_THAT(s.getRadius(), Catch::Matchers::WithinAbs(1.0f, 1e-3));
    REQUIRE_THAT(s2.getRadius(), Catch::Matchers::WithinAbs(2.0f, 1e-3));
}