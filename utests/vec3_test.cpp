#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <renderLib/vec3.h>
#include <iostream>
#include <memory>

TEST_CASE( "Dot Product Test" )
{
    vec3 v = vec3(1,2,3);
    vec3 u = vec3(4,5,6);

    float dot_product = dot(u, v);
    float expected = 32.0f;

    REQUIRE_THAT(dot_product, Catch::Matchers::WithinAbs(expected, 1.0e-3f));
}

TEST_CASE( "Cross Product Test" )
{
    vec3 v = vec3(1,2,3);
    vec3 u = vec3(4,5,6);

    vec3 cross_product = cross(v, u);
    vec3 expected = vec3(-3,6,-3);
    REQUIRE_THAT(cross_product.x(), Catch::Matchers::WithinAbs(expected.x(), 1.0e-3f));
    REQUIRE_THAT(cross_product.y(), Catch::Matchers::WithinAbs(expected.y(), 1.0e-3f));
    REQUIRE_THAT(cross_product.z(), Catch::Matchers::WithinAbs(expected.z(), 1.0e-3f));
}

TEST_CASE( "Length Squared Test" )
{
    vec3 v = vec3(1,2,2);

    float length_sq = v.length_squared();
    float expected = 9.0f;

    REQUIRE_THAT(length_sq, Catch::Matchers::WithinAbs(expected, 1.0e-3f));
}

TEST_CASE( "Unit Vector Test" )
{
    vec3 v = vec3(3,0,4);

    float len = v.length();
    vec3 unitVector = unit_vector(v);
    vec3 expected = vec3(0.6f, 0.0f, 0.8f);

    REQUIRE_THAT(unitVector.x(), Catch::Matchers::WithinAbs(expected.x(), 1.0e-3f));
    REQUIRE_THAT(unitVector.y(), Catch::Matchers::WithinAbs(expected.y(), 1.0e-3f));
    REQUIRE_THAT(unitVector.z(), Catch::Matchers::WithinAbs(expected.z(), 1.0e-3f));
}

TEST_CASE( " Length Test" )
{
    vec3 v = vec3(1,2,2);

    float length = v.length();
    float expected = 3.0f;

    REQUIRE_THAT(length, Catch::Matchers::WithinAbs(expected, 1.0e-3f));
}
